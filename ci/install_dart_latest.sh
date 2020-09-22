if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    sudo apt-get -qq update
    sudo apt-get -y install software-properties-common
    sudo apt-add-repository -y ppa:dartsim/ppa
    sudo apt-get -qq update

    sudo apt-get install -y --no-install-recommends libassimp-dev libccd-dev libfcl-dev
else
    HOMEBREW_NO_AUTO_UPDATE=1 brew install dartsim --only-dependencies
fi

git clone git://github.com/dartsim/dart.git
cd dart
mkdir build
cd build
if [ "$PYTHON_TESTS" = "ON" ]; then
cmake -DDART_BUILD_DARTPY=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
else
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
fi
make -j4
sudo make install
if [ "$PYTHON_TESTS" = "ON" ]; then
    make dartpy -j4
    sudo make install-dartpy
fi
sudo ldconfig
cd $CI_HOME
