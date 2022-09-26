#include "Components/BoxComponent.h"
#include "Engine/CollisionProfile.h"
#include "MovementComponents/DefaultMovementComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"

#include "PhysXPublic.h" // PhysX Conversion to Unreal Engine and vice versa managed by UE
#include "PhysXVehicleManager.h" // PhysX API manager for PhysX Vehicle Class managed by UE
#include "TireConfig.h" // PhysX API for tire config managed by UE
#include "VehicleWheel.h" // PhysX API for wheel config managed by UE

#include "Carla.h"
#include "Carla/Game/CarlaHUD.h"
#include "Carla/Game/CarlaStatics.h"
#include "Carla/Trigger/FrictionTrigger.h"
#include "Carla/Util/ActorAttacher.h"
#include "Carla/Util/EmptyActor.h"
#include "Carla/Util/BoundingBoxCalculator.h"
#include "Carla/Vehicle/CarlaWheeledVehicle.h"

void ACarlaWheeledVehicle::ApplyVehiclePhysicsControl(const FVehiclePhysicsControl &PhysicsControl)
{
  LastPhysicsControl = PhysicsControl;

  UWheeledVehicleMovementComponent4W *Vehicle4W = Cast<UWheeledVehicleMovementComponent4W>(
      GetVehicleMovement());
  check(Vehicle4W != nullptr);

  // Engine Setup value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  Vehicle4W->EngineSetup.TorqueCurve.EditorCurveData = PhysicsControl.TorqueCurve;
  Vehicle4W->EngineSetup.MaxRPM = PhysicsControl.MaxRPM;

  Vehicle4W->EngineSetup.MOI = PhysicsControl.MOI;

  Vehicle4W->EngineSetup.DampingRateFullThrottle = PhysicsControl.DampingRateFullThrottle;
  Vehicle4W->EngineSetup.DampingRateZeroThrottleClutchEngaged =
      PhysicsControl.DampingRateZeroThrottleClutchEngaged;
  Vehicle4W->EngineSetup.DampingRateZeroThrottleClutchDisengaged =
      PhysicsControl.DampingRateZeroThrottleClutchDisengaged;

  // Transmission Setup value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  Vehicle4W->TransmissionSetup.bUseGearAutoBox = PhysicsControl.bUseGearAutoBox;
  Vehicle4W->TransmissionSetup.GearSwitchTime = PhysicsControl.GearSwitchTime;
  Vehicle4W->TransmissionSetup.ClutchStrength = PhysicsControl.ClutchStrength;
  Vehicle4W->TransmissionSetup.FinalRatio = PhysicsControl.FinalRatio;

  TArray<FVehicleGearData> ForwardGears;

  for (const auto &Gear : PhysicsControl.ForwardGears)
  {
    FVehicleGearData GearData;

    GearData.Ratio = Gear.Ratio;
    GearData.UpRatio = Gear.UpRatio;
    GearData.DownRatio = Gear.DownRatio;

    ForwardGears.Add(GearData);
  }

  Vehicle4W->TransmissionSetup.ForwardGears = ForwardGears;

  // Vehicle Setup value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  Vehicle4W->Mass = PhysicsControl.Mass;
  Vehicle4W->DragCoefficient = PhysicsControl.DragCoefficient;

  // Center of mass value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  UPrimitiveComponent *UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
  check(UpdatedPrimitive != nullptr);

  UpdatedPrimitive->BodyInstance.COMNudge = PhysicsControl.CenterOfMass;

  // Transmission Setup value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  Vehicle4W->SteeringCurve.EditorCurveData = PhysicsControl.SteeringCurve;

  // Wheels Setup value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  const int PhysicsWheelsNum = PhysicsControl.Wheels.Num();
  if (PhysicsWheelsNum != 4)
  {
    UE_LOG(LogCarla, Error, TEXT("Number of WheelPhysicsControl is not 4."));
    return;
  }

  // Change, if required, the collision mode for wheels value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  SetWheelCollision(Vehicle4W, PhysicsControl);

  TArray<FWheelSetup> NewWheelSetups = Vehicle4W->WheelSetups;

  for (int32 i = 0; i < PhysicsWheelsNum; ++i)
  {
    UVehicleWheel *Wheel = NewWheelSetups[i].WheelClass.GetDefaultObject();
    check(Wheel != nullptr);

    // Assigning new tire config value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
    Wheel->TireConfig = DuplicateObject<UTireConfig>(Wheel->TireConfig, nullptr);

    // Setting a new value to friction value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
    Wheel->TireConfig->SetFrictionScale(PhysicsControl.Wheels[i].TireFriction);
  }

  Vehicle4W->WheelSetups = NewWheelSetups;

  // Recreate Physics State for vehicle setup value here are imported from UE inside WheeledVehicleMovementComponent4W.h imported from PhysX inside PxVehicleComponents.h
  GetWorld()->GetPhysicsScene()->GetPxScene()->lockWrite();
  Vehicle4W->RecreatePhysicsState();
  GetWorld()->GetPhysicsScene()->GetPxScene()->unlockWrite();

  for (int32 i = 0; i < PhysicsWheelsNum; ++i)
  {
    PxVehicleWheelData PWheelData = Vehicle4W->PVehicle->mWheelsSimData.getWheelData(i);

    PWheelData.mRadius = PhysicsControl.Wheels[i].Radius;
    PWheelData.mMaxSteer = FMath::DegreesToRadians(PhysicsControl.Wheels[i].MaxSteerAngle);
    PWheelData.mDampingRate = M2ToCm2(PhysicsControl.Wheels[i].DampingRate);
    PWheelData.mMaxBrakeTorque = M2ToCm2(PhysicsControl.Wheels[i].MaxBrakeTorque);
    PWheelData.mMaxHandBrakeTorque = M2ToCm2(PhysicsControl.Wheels[i].MaxHandBrakeTorque);
    Vehicle4W->PVehicle->mWheelsSimData.setWheelData(i, PWheelData);

    PxVehicleTireData PTireData = Vehicle4W->PVehicle->mWheelsSimData.getTireData(i);
    PTireData.mLatStiffX = PhysicsControl.Wheels[i].LatStiffMaxLoad;
    PTireData.mLatStiffY = PhysicsControl.Wheels[i].LatStiffValue;
    PTireData.mLongitudinalStiffnessPerUnitGravity = PhysicsControl.Wheels[i].LongStiffValue;
    Vehicle4W->PVehicle->mWheelsSimData.setTireData(i, PTireData);
  }

  auto * Recorder = UCarlaStatics::GetRecorder(GetWorld());
  if (Recorder && Recorder->IsEnabled())
  {
    Recorder->AddPhysicsControl(*this);
  }
}