mkdir -p ~/.deps
cd ~/.deps
git clone git://github.com/dartsim/dart.git
cd dart
git checkout $DART_TAG
mkdir build
cd build
# Select Compiler
if [ "$COMPILER" = "gcc" ]; then
export CC=/usr/bin/gcc && export CXX=/usr/bin/g++
fi
if [ "$COMPILER" = "clang" ]; then
export CC=/usr/bin/clang && export CXX=/usr/bin/clang++
fi
# Build DART
if [ "$BUILD_PYTHON" = "ON" ]; then
cmake -DDART_BUILD_DARTPY=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=~/.dart_install ..
else
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=~/.dart_install ..
fi
make -j4
sudo make install
if [ "$BUILD_PYTHON" = "ON" ]; then
    sudo make install-dartpy
fi
ls ~/.dart_install/lib
ls ~/.dart_install/share
sudo ldconfig
cd $CI_HOME
