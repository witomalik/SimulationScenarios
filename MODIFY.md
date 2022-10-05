# Requirements
- QGIS
- CARLA AD Map (QGIS Plugin)
- CARLA Traffic Generation Editor (QGIS Plugin)


# Installation
To install first we can use the environment.yml to create conda environment with QGIS
```console
conda env create -f environment.yml
```
After creating conda environment we can add "CARLA AD Map" and "CARLA Scenario Generation Editor" plugin into QGIS.

1. Download [CARLA AD Map](https://github.com/carla-simulator/map/releases/download/v2.6.2/ad_map_access_qgis.zip) and [CARLA AD Map SOURCE](https://github.com/carla-simulator/map/archive/refs/tags/v2.6.2.zip)

2. Download [CARLA Traffic Generation Editor](https://github.com/carla-simulator/traffic-generation-editor/archive/refs/heads/main.zip)

3. Open QGIS by activate the conda and run qgis
```console
conda activate NQGIS
QGIS
```

4. Install plugin from plugins menu (Plugins > Manage and Install Plugins > Install from ZIP), choose the zip file that was previously downloaded.

*Note: If CARLA AD Map plugin has not been installed before, same steps can be used to install the ZIP package.*

5. Change to Installed tab and make sure OpenSCENARIO Editor Toolkit is enabled.

# Look Vehicle Global Position
Sometimes in scenarios we need to place the vehicle on correct lane within the CARLA Map. The global position can be seen using QGIS. First we need to open the CARLA Map using CARLA AD Map

Once the plugin is loaded, the CARLA ad_map_access toolbar is now available for use.

1. The map is loaded using the load map icon on the CARLA ad_map_access toolbar. Currently, it is possible to directly load a txt configuration file and xodr files from ad_map_access. In addition it is possible to use the txt configuration file to specify some configuration values. Linking an example file. Also please refer to the configuration structure. Loading the desired map Select the map from the file system

2. The map after successful loading shows all the layers with the Lane Geometry, Lane types, Lane Contact Types, Landmark, Topology and speed limits.

3. Click on 'Add vehicles' button to load dock widget.

4. Choose parameters (vehicle type, use lane heading / manually specify, initial speed)

5. Agent selection is only enabled for Ego vehicles. You can also specify your own agent.

6. Press on 'Insert', mouse pointer will change to cross-type pointer.

7. Click on desired map position to spawn vehicle.

8. If click point contains multiple lanes and 'Use lane heading' is checked, a dialog box will appear and ask to select lane ID to use for lane heading.

9. If click point contains lanes with multiple elevations, a dialog box will appear and ask to select the desired elevation.

*Note: Sometimes there will be a "Click point too far from lane" error. Try choosing a different elevation.*

*Note: Currently you can insert vehicle one at a time.*

*Note: You can toggle labels on and off by clicking on the 'Label' button*

10. In Layers right click on layer and select Open Attribute Table. Here we can see the information of the vehicle position.


# Modify Scenario
To modify secnario we need to use base scenario provided by scenario_runner in examples folder (scenario_runner/srunner/examples). This base scenario is used because CARLA have some parameter that compatible only with this kind of scenario, meanwhile in Open Pilot have more compatibility with many scenarios.

1. Open base scenario files using xml editor e.g. VS Code
2. Modify the scenarios based on what we need


# Documentation
[QGIS](https://qgis.org/en/docs/index.html)
[CARLA AD Map](https://ad-map-access.readthedocs.io/en/latest/ad_map_access_qgis/ad_map_access_qgis/#how-to-use-the-tool)
[CARLA Traffic Generation Editor](https://github.com/carla-simulator/traffic-generation-editor#adding-vehicles)



