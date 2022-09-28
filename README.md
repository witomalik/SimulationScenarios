# SimulationScenarios
All scenarios required to run ALKS scenarios on CARLA. 

# Requirements
- CARLA 0.9.10.1
- Scenario Runner 0.9.10 (Scenario Runner version should match with CARLA version)

[Install Requirements](INSTALL.md)

# Steps
0. Source ~/.bashrc and acticate conda environment
```console
source ~/.bashrc
conda activate CARLA
```
1. Run CARLA
Go to CARLA directory, the location is same with ${CARLA_ROOT}
```console
./CarlaUE4.sh
```
2. Run Scenario Runner
**We need another terminal window**
Go to CARLA directory, the location is same with ${SCENARIO_RUNNER_ROOT}. Scenario runner has many options to run, we can look the options by using -h tag.
```console
python3 scenario_runner.py -h
```
**example**
We will run the first open scenario *x-4-1.xosc*
```console
python3 scenario_runner.py --openscenario ${OPENSCENARIO_FILE} # Change ${OPENSCENARIO_FILE} for your Opoen Scenario path
```
3. Run Manual Control
**We need another terminal window**
Run manual control to attach ego_vehicle camera in the scenario
```console
python3 manual_control.py
```

