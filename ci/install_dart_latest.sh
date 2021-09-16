mkdir -p ~/.deps
cd ~/.deps
git clone git://github.com/dartsim/dart.git
cd dart
git checkout $DART_TAG
mkdir build
cd build
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
ls ~/.dart_install
sudo ldconfig
cd $CI_HOME
