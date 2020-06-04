PY_PACKAGE_FOLDER=dist-packages
PY_VERSION=$(python3 -c "import platform; ver = platform.python_version(); ver_arr = ver.split('.'); print(ver_arr[0])")
if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    sudo apt-get install -y --no-install-recommends \
        python3-dev \
        python3-numpy \
        python3-pip \
        lsb-release

    # python bindings of DART require OSG!!
    sudo apt-get install -y --no-install-recommends libopenscenegraph-dev freeglut3-dev

    if [ $(lsb_release -sc) = "xenial" ] || [ $(lsb_release -sc) = "bionic" ]; then
        git clone https://github.com/pybind/pybind11 -b 'v2.3.0' --single-branch --depth 1
        cd pybind11
        mkdir build
        cd build
        pythonVersion=python3.6
        if [ $(lsb_release -sc) = "xenial" ]; then
            pythonVersion=python3.5
        fi
        cmake .. -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE=$(python-config --prefix)/bin/${pythonVersion} -DPYTHON_LIBRARY=$(python-config --prefix)/lib/lib${pythonVersion}m.so -DPYTHON_INCLUDE_DIR=$(python-config --prefix)/include/${pythonVersion}m -DPYBIND11_TEST=OFF
        make -j4
        sudo make install
        cd ../..
    elif [ $(lsb_release -sc) = "eoan" ] || [ $(lsb_release -sc) = "focal" ]; then
        sudo apt-get -y install pybind11-dev python3 libpython3-dev python3-pytest \
            python3-distutils
    else
        echo -e "$(lsb_release -sc) is not supported."
        exit 1
    fi
else
    HOMEBREW_NO_AUTO_UPDATE=1 brew install python
    HOMEBREW_NO_AUTO_UPDATE=1 brew install pybind11

    sudo -H pip3 install numpy

    PY_PACKAGE_FOLDER=site-packages
    PY_VERSION=$(python3 -c "import platform; ver = platform.python_version(); ver_arr = ver.split('.'); print(ver_arr[0]+'.'+ver_arr[1])")
fi

PY_PACKAGES=/usr/local/lib/python${PY_VERSION}/${PY_PACKAGE_FOLDER}
export PYTHONPATH="${PYTHONPATH}:${PY_PACKAGES}"
