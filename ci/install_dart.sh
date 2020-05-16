sudo apt-add-repository ppa:dartsim/ppa -y
sudo apt-get -qq update

sudo apt-get -qq --yes --force-yes install libdart6-dev libdart-utils-urdf-dev python3-dartpy

if [ "$DART_MASTER" = "OFF" && "$DART_EXTRA" = "ON" ]; then 'sudo apt-get -qq --yes --force=yes install libdart-collision-ode-dev libdart-collision-bullet-dev' ; fi
