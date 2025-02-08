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
git checkout tags/v6.13.2
if [ -d "build" ] # In case of a previous attempt that has not been cleaned
then
  sudo rm -rf build
fi
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/dart -DDART_BUILD_DARTPY=ON ..
make -j8 dartpy
sudo make install dartpy

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
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DMAGNUM_WITH_AUDIO=ON -DMAGNUM_WITH_DEBUGTOOLS=ON -DMAGNUM_WITH_GL=ON -DMAGNUM_WITH_MESHTOOLS=ON -DMAGNUM_WITH_PRIMITIVES=ON -DMAGNUM_WITH_SCENEGRAPH=ON -DMAGNUM_WITH_SHADERS=ON -DMAGNUM_WITH_TEXT=ON -DMAGNUM_WITH_TEXTURETOOLS=ON -DMAGNUM_WITH_TRADE=ON -DMAGNUM_WITH_GLFWAPPLICATION=ON -DMAGNUM_WITH_WINDOWLESSGLXAPPLICATION=ON  -DMAGNUM_WITH_WINDOWLESSCGLAPPLICATION=ON -DMAGNUM_WITH_OPENGLTESTER=ON -DMAGNUM_WITH_ANYAUDIOIMPORTER=ON -DMAGNUM_WITH_ANYIMAGECONVERTER=ON -DMAGNUM_WITH_ANYIMAGEIMPORTER=ON -DMAGNUM_WITH_ANYSCENEIMPORTER=ON -DMAGNUM_WITH_MAGNUMFONT=ON -DMAGNUM_WITH_OBJIMPORTER=ON -DMAGNUM_WITH_TGAIMPORTER=ON -DMAGNUM_WITH_WAVAUDIOIMPORTER=ON -DMAGNUM_WITH_GL_INFO=ON -DMAGNUM_WITH_AL_INFO=ON -DMAGNUM_TARGET_EGL=OFF ..
make -j8
sudo make install

cd ../..
if [ ! -d "magnum-plugins" ]
then
git clone https://github.com/mosra/magnum-plugins.git
fi
cd magnum-plugins
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DMAGNUM_WITH_ASSIMPIMPORTER=ON -DMAGNUM_WITH_DDSIMPORTER=ON -DMAGNUM_WITH_JPEGIMPORTER=ON -DMAGNUM_WITH_OPENGEXIMPORTER=ON -DMAGNUM_WITH_PNGIMPORTER=ON -DMAGNUM_WITH_TINYGLTFIMPORTER=ON -DMAGNUM_WITH_STBTRUETYPEFONT=ON ..
make -j
sudo make install

cd ../..
if [ ! -d "magnum-integration" ]
then
git clone https://github.com/mosra/magnum-integration.git
fi
cd magnum-integration
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DMAGNUM_WITH_DART=ON -DMAGNUM_WITH_EIGEN=ON -DMAGNUM_WITH_BULLET=ON -DMAGNUM_WITH_GLM=ON ..
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
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum -DMAGNUM_WITH_PYTHON=ON ..
make -j
cd src/python
sudo python3 setup.py install --root=/opt/magnum/lib --install-purelib=python3/site-packages --install-platlib=python3/site-packages --install-scripts=python3/scripts --install-headers=python3/include --install-data=python3/data

cd ../../../../../../..

export PYTHONPATH=/opt/magnum/lib/python3/site-packages:$PYTHONPATH

# RobotDART
python3 waf configure --prefix /opt/robot_dart --python --dart /opt/dart --magnum /opt/magnum
python3 waf -j8
python3 waf examples -j8
sudo python3 waf install
