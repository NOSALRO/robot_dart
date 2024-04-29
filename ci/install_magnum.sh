# Select Compiler
if [ "$COMPILER" = "gcc" ]; then
export CC=/usr/bin/gcc && export CXX=/usr/bin/g++
fi
if [ "$COMPILER" = "clang" ]; then
export CC=/usr/bin/clang && export CXX=/usr/bin/clang++
fi

PREFIX=/usr

PREFIX=${1:-$PREFIX}

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    CYGWIN*)    machine=Cygwin;;
    MINGW*)     machine=MinGw;;
    *)          machine="UNKNOWN:${unameOut}"
esac

echo "$machine"

WINDOWLESS=DMAGNUM_WITH_WINDOWLESSEGLAPPLICATION

if [ "$machine" == "Mac" ]; then
    WINDOWLESS=DMAGNUM_WITH_WINDOWLESSCGLAPPLICATION
fi

mkdir -p ~/.deps
cd ~/.deps
# install Corrade
git clone https://github.com/mosra/corrade.git
cd corrade
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
make -j
sudo make install
cd ../..

# install Magnum
git clone https://github.com/mosra/magnum.git
cd magnum
mkdir build && cd build
# Ubuntu
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMAGNUM_WITH_AUDIO=ON -DMAGNUM_WITH_DEBUGTOOLS=ON -DMAGNUM_WITH_GL=ON -DMAGNUM_WITH_MESHTOOLS=ON -DMAGNUM_WITH_PRIMITIVES=ON -DMAGNUM_WITH_SCENEGRAPH=ON -DMAGNUM_WITH_SHADERS=ON -DMAGNUM_WITH_TEXT=ON -DMAGNUM_WITH_TEXTURETOOLS=ON -DMAGNUM_WITH_TRADE=ON -DMAGNUM_WITH_GLFWAPPLICATION=ON -${WINDOWLESS}=ON -DMAGNUM_WITH_OPENGLTESTER=ON -DMAGNUM_WITH_ANYAUDIOIMPORTER=ON -DMAGNUM_WITH_ANYIMAGECONVERTER=ON -DMAGNUM_WITH_ANYIMAGEIMPORTER=ON -DMAGNUM_WITH_ANYSCENEIMPORTER=ON -DMAGNUM_WITH_MAGNUMFONT=ON -DMAGNUM_WITH_OBJIMPORTER=ON -DMAGNUM_WITH_TGAIMPORTER=ON -DMAGNUM_WITH_WAVAUDIOIMPORTER=ON -DMAGNUM_TARGET_EGL=ON -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
make -j
sudo make install
cd ../..

# install MagnumPlugins
git clone https://github.com/mosra/magnum-plugins.git
cd magnum-plugins
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMAGNUM_WITH_ASSIMPIMPORTER=ON -DMAGNUM_WITH_DDSIMPORTER=ON -DMAGNUM_WITH_JPEGIMPORTER=ON -DMAGNUM_WITH_OPENGEXIMPORTER=ON -DMAGNUM_WITH_PNGIMPORTER=ON -DMAGNUM_WITH_TINYGLTFIMPORTER=ON -DMAGNUM_WITH_STBTRUETYPEFONT=ON  -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
make -j
sudo make install
cd ../..

# install MagnumIntegration
git clone https://github.com/mosra/magnum-integration.git
cd magnum-integration
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMAGNUM_WITH_DART=ON -DMAGNUM_WITH_EIGEN=ON  -DCMAKE_INSTALL_PREFIX=${PREFIX} -DDART_DIR=$DART_DIR ..
make -j
sudo make install
cd ../..

if [ "$PREFIX" = "/usr" ]; then
sudo ldconfig
fi
cd $CI_HOME
