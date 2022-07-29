set -x
# sudo apt-add-repository -y ppa:dartsim/ppa
# sudo apt update
# sudo apt install -y build-essential cmake pkg-config git
# sudo apt install -y libeigen3-dev libccd-dev libfcl-dev libboost-regex-dev libboost-system-dev libbullet-dev libode-dev liboctomap-dev
# sudo apt install -y libtinyxml-dev libtinyxml2-dev
# sudo apt install -y liburdfdom-dev liburdfdom-headers-dev python3-pip python3-numpy
# sudo apt install -y libxi-dev libxmu-dev freeglut3-dev libopenscenegraph-dev
# sudo apt install -y libassimp-dev pybind11-dev
# sudo apt install -y libopenal-dev libglfw3-dev libsdl2-dev libopenexr-dev
# sudo apt install -y libdevil-dev libpng-dev libfaad-dev libfreetype6-dev libglm-dev
# sudo apt install -y python-is-python3


brew install cmake eigen fcl tinyxml tinyxml2 urdfdom assimp boost numpy
python -m pip install numpy pytest

# Remove previous attempts
sudo rm -rf /opt/magnum
sudo rm -rf /opt/dart
sudo rm -rf /opt/robot_dart

# Make temp folder
#rm -rf temp_robot_dart
mkdir -p temp_robot_dart
cd temp_robot_dart

# DART
git clone -b v6.12.1 --single-branch --depth 1 https://github.com/dartsim/dart.git
cd dart && git checkout v6.12.1
mkdir -p build && cd build
cmake -DDART_BUILD_DARTPY=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/dart ..
make -j8
sudo make install
cd ../..
pwd

export LD_LIBRARY_PATH=/opt/dart/lib:$LD_LIBRARY_PATH
sudo ln -s /opt/dart/lib/python3.9 /opt/dart/lib/python3  # a bit hacky...
export PYTHONPATH=/opt/dart/lib/python3/site-packages:$PYTHONPATH

# Magnum related
git clone  --depth 1 https://github.com/mosra/corrade.git
cd corrade
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum ..
make -j8
sudo make install
cd ../..
pwd

git clone  --depth 1 https://github.com/mosra/magnum.git
cd magnum
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DMAGNUM_WITH_AUDIO=ON -DMAGNUM_WITH_DEBUGTOOLS=ON -DMAGNUM_WITH_GL=ON -DMAGNUM_WITH_MESHTOOLS=ON -DMAGNUM_WITH_PRIMITIVES=ON -DMAGNUM_WITH_SCENEGRAPH=ON -DMAGNUM_WITH_SHADERS=ON -DMAGNUM_WITH_TEXT=ON -DMAGNUM_WITH_TEXTURETOOLS=ON -DMAGNUM_WITH_TRADE=ON -DMAGNUM_WITH_GLFWAPPLICATION=ON -DMAGNUM_WITH_WINDOWLESSGLXAPPLICATION=ON  -DMAGNUM_WITH_WINDOWLESSEGLAPPLICATION=OFF -DMAGNUM_WITH_OPENGLTESTER=ON -DMAGNUM_WITH_ANYAUDIOIMPORTER=ON -DMAGNUM_WITH_ANYIMAGECONVERTER=ON -DMAGNUM_WITH_ANYIMAGEIMPORTER=ON -DMAGNUM_WITH_ANYSCENEIMPORTER=ON -DMAGNUM_WITH_MAGNUMFONT=ON -DMAGNUM_WITH_OBJIMPORTER=ON -DMAGNUM_WITH_TGAIMPORTER=ON -DMAGNUM_WITH_WAVAUDIOIMPORTER=ON -DMAGNUM_WITH_GL_INFO=ON -DMAGNUM_WITH_AL_INFO=ON -DMAGNUM_WITH_EGLCONTEXT=OFF ..
make -j8
sudo make install
cd ../..
pwd

git clone  --depth 1 https://github.com/mosra/magnum-plugins.git
cd magnum-plugins
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DMAGNUM_WITH_ASSIMPIMPORTER=ON -DMAGNUM_WITH_DDSIMPORTER=ON -DMAGNUM_WITH_JPEGIMPORTER=ON -DMAGNUM_WITH_OPENGEXIMPORTER=ON -DMAGNUM_WITH_PNGIMPORTER=ON -DMAGNUM_WITH_TINYGLTFIMPORTER=ON -DMAGNUM_WITH_STBTRUETYPEFONT=ON ..
make -j
sudo make install
cd ../..

git clone  --depth 1 https://github.com/mosra/magnum-integration.git
cd magnum-integration
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DMAGNUM_WITH_DART=ON -DMAGNUM_WITH_EIGEN=ON -DMAGNUM_WITH_BULLET=ON -DMAGNUM_WITH_GLM=ON ..
make -j
sudo make install
cd ../..
pwd

export PATH=/opt/magnum/bin:$PATH
export LD_LIBRARY_PATH=/opt/magnum/lib:$LD_LIBRARY_PATH

# sudo ln -s /usr/bin/python3 /usr/bin/python
pwd
cd ../
pwd
rm -rf temp_robot_dart
cd ..
pwd
# RobotDART
./waf configure --prefix /opt/robot_dart --python --dart /opt/dart --corrade_install_dir /opt/magnum --magnum_install_dir /opt/magnum --magnum_plugins_install_dir /opt/magnum --magnum_integration_install_dir /opt/magnum
./waf -j8
./waf examples -j8
#sudo ./waf install

