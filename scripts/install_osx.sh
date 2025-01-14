set -e

CLEAN=0

CLEAN=${1:-$CLEAN}

brew install binutils pybind11
brew install numpy python-setuptools

brew install dartsim --only-dependencies

brew install sdl2 glfw eigen glm bullet assimp devil faad2 freetype glslang harfbuzz libpng libspng jpeg openexr spirv-tools zstd webp

if [ $CLEAN -ne 0 ]; then
    echo "-- Cleaning.."
    # Remove previous attempts
    sudo rm -rf /opt/magnum
    sudo rm -rf /opt/dart
    sudo rm -rf /opt/robot_dart
fi

# Make temp folder
mkdir -p temp_robot_dart
cd temp_robot_dart

if [ $CLEAN -ne 0 ]; then
    sudo rm -rf dart
fi

# DART related
if [ ! -d "dart" ]
then
git clone https://github.com/dartsim/dart.git
fi
cd dart
git checkout tags/v6.15.0
if [ -d "build" ] # In case of a previous attempt that has not been cleaned
then
  sudo rm -rf build
fi
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/dart -DDART_BUILD_DARTPY=ON ..
make -j8 dartpy
sudo make install dartpy
python -m pip install ..

export LD_LIBRARY_PATH=/opt/dart/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/opt/dart/lib:$DYLD_LIBRARY_PATH
export PYTHONPATH=/opt/dart:/opt/dart/lib/python3/dist-packages:$PYTHONPATH

if [ $CLEAN -ne 0 ]; then
    sudo rm -rf corrade
    sudo rm -rf magnum
    sudo rm -rf magnum-plugins
    sudo rm -rf magnum-integration
    sudo rm -rf magnum-bindings
fi

# Magnum related
if [ ! -d "corrade" ]
then
git clone https://github.com/mosra/corrade.git
fi
cd corrade
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum ..
make -j8
sudo make install

cd ../..
if [ ! -d "magnum" ]
then
git clone https://github.com/mosra/magnum.git
fi
cd magnum
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_AUDIO=ON -DWITH_DEBUGTOOLS=ON -DWITH_GL=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_SCENEGRAPH=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_TEXTURETOOLS=ON -DWITH_TRADE=ON -DWITH_GLFWAPPLICATION=ON -DWITH_WINDOWLESSCGLAPPLICATION=ON -DWITH_OPENGLTESTER=ON -DWITH_ANYAUDIOIMPORTER=ON -DWITH_ANYIMAGECONVERTER=ON -DWITH_ANYIMAGEIMPORTER=ON -DWITH_ANYSCENEIMPORTER=ON -DWITH_MAGNUMFONT=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_WAVAUDIOIMPORTER=ON ..
make -j8
sudo make install

cd ../..
if [ ! -d "magnum-plugins" ]
then
git clone https://github.com/mosra/magnum-plugins.git
fi
cd magnum-plugins
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DWITH_ASSIMPIMPORTER=ON -DWITH_DDSIMPORTER=ON -DWITH_JPEGIMPORTER=ON -DWITH_OPENGEXIMPORTER=ON -DWITH_PNGIMPORTER=ON -DWITH_TINYGLTFIMPORTER=ON -DWITH_STBTRUETYPEFONT=ON ..
make -j
sudo make install

cd ../..
if [ ! -d "magnum-integration" ]
then
git clone https://github.com/mosra/magnum-integration.git
fi
cd magnum-integration
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DWITH_DART=ON -DWITH_EIGEN=ON -DWITH_BULLET=ON -DWITH_GLM=ON ..
make -j
sudo make install

export PATH=/opt/magnum/bin:$PATH
export LD_LIBRARY_PATH=/opt/magnum/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/opt/magnum/lib:$DYLD_LIBRARY_PATH

cd ../..
if [ ! -d "magnum-bindings" ]
then
git clone https://github.com/mosra/magnum-bindings.git
fi
cd magnum-bindings
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DWITH_PYTHON=ON ..
make -j
cd src/python
sudo python3 setup.py install --root=/opt/magnum/lib --install-purelib=python3/site-packages --install-platlib=python3/site-packages --install-scripts=python3/scripts --install-headers=python3/include --install-data=python3/data

cd ../../../../../../..

export PYTHONPATH=/opt/magnum/lib/python3/site-packages:$PYTHONPATH

# RobotDART
python3 waf configure --prefix /opt/robot_dart --dart /opt/dart --magnum /opt/magnum --python
sudo python3 waf -j8
sudo python3 waf examples -j8
sudo python3 waf install
