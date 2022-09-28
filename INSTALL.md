# Get CARLA 0.9.10.1
1. Download [CARLA 0.9.10.1](https://carla-releases.s3.eu-west-3.amazonaws.com/Linux/CARLA_0.9.10.1.tar.gz)

2. Unzip the package into a folder, e.g. CARLA.

3. Install some dependencies for PythonAPI environment. As a reference, for conda, start by creating a new environment:
```console
conda create -n py37 python=3.7
conda activate py37

cd ${CARLA_ROOT}  # Change ${CARLA_ROOT} for your CARLA root folder e.g. CARLA.

pip3 install -r PythonAPI/carla/requirements.txt
```
Download [additional maps](https://carla-releases.s3.eu-west-3.amazonaws.com/Linux/AdditionalMaps_0.9.10.1.tar.gz) and move the package to the Import folder, and run the following script to extract them.
```console
cd ${CARLA_ROOT}  # Change ${CARLA_ROOT} for your CARLA root folder e.g. CARLA.

./ImportAssets.sh
```

Make sure to download 0.9.10.1 and its corresponding maps. This is the exact version used by the online servers.

# Get the Leaderboard and Scenario_Runner
1. Download the Leaderboard repository.
```console
git clone -b stable --single-branch https://github.com/carla-simulator/leaderboard.git
```

2. Install the required Python dependencies.
```console
cd ${LEADERBOARD_ROOT} # Change ${LEADERBOARD_ROOT} for your Leaderboard root folder

pip3 install -r requirements.txt
```
3. Download the Scenario_Runner repository.
```console
git clone -b leaderboard --single-branch https://github.com/carla-simulator/scenario_runner.git
```

Install the required Python dependencies using the same Python environments.
```console
cd ${SCENARIO_RUNNER_ROOT} # Change ${SCENARIO_RUNNER_ROOT} for your Scenario_Runner root folder

pip3 install -r requirements.txt
```

# Define the environment variables
We need to make sure that the different modules can find each other.

Open the visual studio code in home directory with the following command:
```console
code ~/
```
Edit your ~/.bashrc profile, adding the definitions below. Save and close the file after editing.
```
export CARLA_ROOT=PATH_TO_CARLA_ROOT
export SCENARIO_RUNNER_ROOT=PATH_TO_SCENARIO_RUNNER
export LEADERBOARD_ROOT=PATH_TO_LEADERBOARD
export PYTHONPATH="${CARLA_ROOT}/PythonAPI/carla/":"${SCENARIO_RUNNER_ROOT}":"${LEADERBOARD_ROOT}":"${CARLA_ROOT}/PythonAPI/carla/dist/carla-0.9.10-py3.7-linux-x86_64.egg":${PYTHONPATH}
```
Remember to source .bashrc for these changes to take effect using the following command:
```console
source ~/.bashrc
```
