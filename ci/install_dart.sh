if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    sudo apt-get -qq update
    sudo apt-get -y install software-properties-common
    sudo apt-add-repository -y ppa:dartsim/ppa
    sudo apt-get -qq update

    sudo apt-get install -y --no-install-recommends libassimp-dev libccd-dev libfcl-dev

    sudo apt-get install -y --no-install-recommends libdart6-dev libdart6-utils-urdf-dev #python3-dartpy

    # libdart6-dev installs a config with ode and bullet on
    sudo apt-get install -y --no-install-recommends libdart6-collision-ode-dev libdart6-collision-bullet-dev
else
    HOMEBREW_NO_AUTO_UPDATE=1 brew install dartsim
    if [ "$PYTHON_TESTS" = "ON" ]; then
        # HOMEBREW_NO_AUTO_UPDATE=1 brew install dartpy
        git clone git://github.com/dartsim/dart.git
        cd dart
        mkdir build_py
        cd build_py
        cmake -DDART_BUILD_DARTPY=ON -DCMAKE_BUILD_TYPE=Release ..
        make -j4
        sudo make install
    fi
fi