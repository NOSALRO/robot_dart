sudo apt-get -qq update
sudo apt-get -y install software-properties-common
sudo apt-add-repository -y ppa:dartsim/ppa
sudo apt-get -qq update

sudo apt-get -qq --yes --force-yes install libassimp-dev libccd-dev libfcl-dev

sudo apt-get -qq --yes --force-yes install libdart6-dev libdart6-utils-urdf-dev #python3-dartpy

if [ "$DART_MASTER" = "OFF" ] && [ "$DART_EXTRA" = "ON" ]; then 'sudo apt-get -qq --yes --force=yes install libdart-collision-ode-dev libdart-collision-bullet-dev' ; fi
