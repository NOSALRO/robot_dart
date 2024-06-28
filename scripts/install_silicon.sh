set -e

CLEAN=0

CLEAN=${1:-$CLEAN}

brew install binutils pybind11
python -m pip install --upgrade pip
pip3 install numpy

brew install dartsim --only-dependencies

brew install sdl2 glfw eigen glm bullet assimp devil faad2 freetype glslang harfbuzz libpng libspng jpeg openexr spirv-tools zstd webp

if ! grep -q '#include <stdio.h>' '/opt/homebrew/Cellar/jpeg-turbo/3.0.3/include/jpeglib.h'; then
    sed -i '' '1s/^/ #include <stdio.h>\n/' /opt/homebrew/Cellar/jpeg-turbo/3.0.3/include/jpeglib.h
fi

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
    rm -rf dart
    rm -rf corrade
    rm -rf magnum
    rm -rf magnum-plugins
    rm -rf magnum-integration
fi

# dart related
if [ ! -d "dart" ]
then
git clone https://github.com/dartsim/dart.git
fi
cd dart
git checkout tags/v6.13.2 
if [ -d "build" ] # In case of a previous attempt that has not been clean 
then
  rm -rf build
fi
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/dart -DDART_BUILD_DARTPY=ON ..
make -j8 dartpy
sudo make install dartpy

cd ../..
# Magnum related
if [ ! -d "corrade" ]
then
git clone https://github.com/mosra/corrade.git
fi
cd corrade
if [ -d "build" ] # In case of a previous attempt that has not been clean 
then
  rm -rf build
fi
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/magnum ..
make -j8
sudo make install

cd ../..
if [ ! -d "magnum" ]
then
git clone https://github.com/mosra/magnum.git
fi
cd magnum
if [ -d "build" ] # In case of a previous attempt that has not been clean 
then
  rm -rf build
fi
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/magnum -DCMAKE_BUILD_TYPE=Release -DWITH_AUDIO=ON -DWITH_DEBUGTOOLS=ON -DWITH_GL=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_SCENEGRAPH=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_TEXTURETOOLS=ON -DWITH_TRADE=ON -DWITH_GLFWAPPLICATION=ON -DWITH_WINDOWLESSCGLAPPLICATION=ON -DWITH_OPENGLTESTER=ON -DWITH_ANYAUDIOIMPORTER=ON -DWITH_ANYIMAGECONVERTER=ON -DWITH_ANYIMAGEIMPORTER=ON -DWITH_ANYSCENEIMPORTER=ON -DWITH_MAGNUMFONT=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_WAVAUDIOIMPORTER=ON ..

make -j8
sudo make install

cd ../..
if [ ! -d "magnum-plugins" ]
then
git clone https://github.com/mosra/magnum-plugins.git
fi
cd magnum-plugins
if [ -d "build" ] # In case of a previous attempt that has not been clean
then
  rm -rf build
fi
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_ASSIMPIMPORTER=ON -DWITH_DDSIMPORTER=ON -DWITH_JPEGIMPORTER=ON -DWITH_OPENGEXIMPORTER=ON -DWITH_PNGIMPORTER=ON -DWITH_TINYGLTFIMPORTER=ON -DWITH_STBTRUETYPEFONT=ON -DCMAKE_INSTALL_PREFIX=/opt/magnum ..

make -j
sudo make install

cd ../..
if [ ! -d "magnum-integration" ]
then
git clone https://github.com/mosra/magnum-integration.git
fi
cd magnum-integration
if [ -d "build" ] # In case of a previous attempt that has not been clean
then
  rm -rf build
fi
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_DART=ON -DWITH_EIGEN=ON -DCMAKE_INSTALL_PREFIX=/opt/magnum ..

make -j
sudo make install

export LD_LIBRARY_PATH=/opt/magnum/lib:/opt/dart/lib
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH

export PYTHONPATH=/opt/dart/

cd ../../..
if [ $CLEAN -ne 0 ]; then
    rm -rf temp_robot_dart
fi
echo Install third parties finished

# RobotDART
python3 ./waf configure --prefix /opt/robot_dart --python --dart /opt/dart/ --corrade_install_dir /opt/magnum --magnum_install_dir /opt/magnum --magnum_plugins_install_dir /opt/magnum --magnum_integration_install_dir /opt/magnum
python3 ./waf -j8
python3 ./waf examples -j8
sudo python3 ./waf install
