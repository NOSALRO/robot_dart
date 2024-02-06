set -x

brew install binutils pybind11
python -m pip install --upgrade pip
pip3 install numpy

brew install dartsim --only-dependencies
pip3 install dartpy

# Remove previous attempts
sudo rm -rf /opt/robot_dart

# RobotDART
./waf configure --prefix /opt/robot_dart --python
./waf -j8
./waf examples -j8
sudo ./waf install
