# Select Compiler
if [ "$COMPILER" = "gcc" ]; then
export CC=/usr/bin/gcc && export CXX=/usr/bin/g++
fi
if [ "$COMPILER" = "clang" ]; then
export CC=/usr/bin/clang && export CXX=/usr/bin/clang++
fi

PREFIX=/usr
DEPS_FOLDER=~/.deps
SUDOCMD=sudo

PREFIX=${1:-$PREFIX}
DEPS_FOLDER=${2:-$DEPS_FOLDER}
SUDOCMD=${3:-$SUDOCMD}

eval SUDOCMD="${SUDOCMD}"

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    CYGWIN*)    machine=Cygwin;;
    MINGW*)     machine=MinGw;;
    *)          machine="UNKNOWN:${unameOut}"
esac

WINDOWLESS=DMAGNUM_WITH_WINDOWLESSEGLAPPLICATION
EGLTARGET=ON
PNGIMPORTER=ON

if [ "$machine" == "Mac" ]; then
    WINDOWLESS=DMAGNUM_WITH_WINDOWLESSCGLAPPLICATION
    EGLTARGET=OFF
    # Disable PngImporter for CI, since it is failing for libpng on Mac osx 12 and master of Magnum!
    PNGIMPORTER=OFF
fi

mkdir -p ${DEPS_FOLDER}
cd ${DEPS_FOLDER}
# install Corrade
if [ ! -d "corrade" ]
then
git clone https://github.com/mosra/corrade.git
cd corrade
else
cd corrade
git pull
fi
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
make -j
${SUDOCMD} make install
cd ../..

# install Magnum
if [ ! -d "magnum" ]
then
git clone https://github.com/mosra/magnum.git
cd magnum
else
cd magnum
git pull
fi
mkdir -p build && cd build
# Ubuntu/OSX
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMAGNUM_WITH_AUDIO=ON -DMAGNUM_WITH_DEBUGTOOLS=ON -DMAGNUM_WITH_GL=ON -DMAGNUM_WITH_MESHTOOLS=ON -DMAGNUM_WITH_PRIMITIVES=ON -DMAGNUM_WITH_SCENEGRAPH=ON -DMAGNUM_WITH_SHADERS=ON -DMAGNUM_WITH_TEXT=ON -DMAGNUM_WITH_TEXTURETOOLS=ON -DMAGNUM_WITH_TRADE=ON -DMAGNUM_WITH_SDL2APPLICATION=ON -DMAGNUM_WITH_GLFWAPPLICATION=ON -${WINDOWLESS}=ON -DMAGNUM_WITH_OPENGLTESTER=ON -DMAGNUM_WITH_ANYAUDIOIMPORTER=ON -DMAGNUM_WITH_ANYIMAGECONVERTER=ON -DMAGNUM_WITH_ANYIMAGEIMPORTER=ON -DMAGNUM_WITH_ANYSCENEIMPORTER=ON -DMAGNUM_WITH_MAGNUMFONT=ON -DMAGNUM_WITH_OBJIMPORTER=ON -DMAGNUM_WITH_TGAIMPORTER=ON -DMAGNUM_WITH_WAVAUDIOIMPORTER=ON -DMAGNUM_TARGET_EGL=${EGLTARGET} -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
make -j
${SUDOCMD} make install
cd ../..

# install Magnum Bindings
if [ ! -d "magnum-bindings" ]
then
git clone https://github.com/mosra/magnum-bindings.git
cd magnum-bindings
else
cd magnum-bindings
git pull
fi
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMAGNUM_WITH_GLFWAPPLICATION=ON -${WINDOWLESS}=ON -DMAGNUM_TARGET_EGL=${EGLTARGET} -DMAGNUM_WITH_PYTHON=ON -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
make -j
${SUDOCMD} make install
cd src/python
${SUDOCMD} python setup.py install --root=${PREFIX} --prefix=''
cd ../../../..

# install MagnumPlugins
if [ ! -d "magnum-plugins" ]
then
git clone https://github.com/mosra/magnum-plugins.git
cd magnum-plugins
else
cd magnum-plugins
git pull
fi
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMAGNUM_WITH_ASSIMPIMPORTER=ON -DMAGNUM_WITH_DDSIMPORTER=ON -DMAGNUM_WITH_JPEGIMPORTER=ON -DMAGNUM_WITH_OPENGEXIMPORTER=ON -DMAGNUM_WITH_PNGIMPORTER=${PNGIMPORTER} -DMAGNUM_WITH_TINYGLTFIMPORTER=ON -DMAGNUM_WITH_STBTRUETYPEFONT=ON  -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
make -j
${SUDOCMD} make install
cd ../..

# install MagnumIntegration
if [ ! -d "magnum-integration" ]
then
git clone https://github.com/mosra/magnum-integration.git
cd magnum-integration
else
cd magnum-integration
git pull
fi
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMAGNUM_WITH_DART=ON -DMAGNUM_WITH_EIGEN=ON  -DCMAKE_INSTALL_PREFIX=${PREFIX} -DDART_DIR=$DART_DIR ..
make -j
${SUDOCMD} make install
cd ../..

if [ "$PREFIX" = "/usr" ]; then
sudo ldconfig
fi
cd $CI_HOME
