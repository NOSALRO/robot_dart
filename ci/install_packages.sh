set -e

sudo apt-get -qq update
sudo apt-get -y install software-properties-common mlocate
sudo apt-add-repository -y ppa:dartsim/ppa
sudo apt-get -qq update
sudo apt-get install -y --no-install-recommends build-essential cmake pkg-config git libeigen3-dev libccd-dev libfcl-dev libboost-regex-dev libboost-system-dev libboost-filesystem-dev libboost-test-dev libbullet-dev libode-dev liboctomap-dev libtinyxml-dev libtinyxml2-dev liburdfdom-dev liburdfdom-headers-dev python3-pip python3-numpy libpython3-dev libxi-dev libxmu-dev freeglut3-dev libopenscenegraph-dev libassimp-dev libfmt-dev pybind11-dev

sudo ln -s /usr/bin/python3 /usr/bin/python

if [ "$MAGNUM_GUI" = "ON" ]; then
    sudo apt-get install -y --no-install-recommends libopenal-dev libglfw3-dev libsdl2-dev libdevil-dev libpng-dev libfaad-dev libfreetype6-dev libglm-dev
fi
