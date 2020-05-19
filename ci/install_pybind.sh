sudo apt-get install -y --no-install-recommends \
    python3-dev \
    python3-numpy \
    python3-pip \
    lsb-release

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