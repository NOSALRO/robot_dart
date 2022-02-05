mkdir -p ~/.deps
cd ~/.deps

python_dist_dir=$(python -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True, prefix=''))")
mkdir -p ~/.dart_install/$python_dist_dir

# Select Compiler
if [ "$COMPILER" = "gcc" ]; then
export CC=/usr/bin/gcc && export CXX=/usr/bin/g++
fi
if [ "$COMPILER" = "clang" ]; then
export CC=/usr/bin/clang && export CXX=/usr/bin/clang++
fi

# Build pybind11 if needed; for clang we need at least v2.5.0
if [ "$COMPILER" = "clang" ]; then
    git clone https://github.com/pybind/pybind11 -b 'v2.5.0' --single-branch --depth 1
    cd pybind11
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DPYBIND11_TEST=OFF
    make -j4
    sudo make install
    cd ../..
fi

git clone git://github.com/dartsim/dart.git
cd dart
git checkout $DART_TAG
mkdir build
cd build

# Build DART
if [ "$BUILD_PYTHON" = "ON" ]; then
cmake -DDART_BUILD_DARTPY=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=/home/runner/.dart_install ..
else
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=/home/runner/.dart_install ..
fi
make -j4
sudo make install
# We do not need this anymore
# if [ "$BUILD_PYTHON" = "ON" ] && [ "$DART_TAG" = "v6.11.1"]; then
#     sudo make install-dartpy
# fi
sudo ldconfig
cd $CI_HOME

# Hack for dartpy installation
python_file=/usr/lib/python3/dist-packages/dartpy.cpython-38-x86_64-linux-gnu.so
if [ -f "$python_file" ]; then
cp $python_file ~/.dart_install/$python_dist_dir
fi
