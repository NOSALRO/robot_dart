sudo apt-get -qq update
sudo apt-get -y install software-properties-common
sudo apt-add-repository -y ppa:dartsim/ppa
sudo apt-get -qq update

sudo apt-get install -y --no-install-recommends libassimp-dev libccd-dev libfcl-dev

sudo apt-get install -y --no-install-recommends libdart6-dev libdart6-utils-urdf-dev #python3-dartpy

# libdart6-dev installs a config with ode and bullet on
sudo apt-get install -y --no-install-recommends libdart6-collision-ode-dev libdart6-collision-bullet-dev
