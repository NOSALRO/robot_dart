mkdir -p ~/.deps
cd ~/.deps

python_dist_dir=$(python -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True, prefix=''))")
mkdir -p ~/.dart_install/lib/$python_dist_dir

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
cmake -DDART_BUILD_DARTPY=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=/home/runner/.dart_install ..
else
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=/home/runner/.dart_install ..
fi
make -j4
sudo make install
if [ "$BUILD_PYTHON" = "ON" ]; then
    sudo make install-dartpy
fi
sudo ldconfig
cd $CI_HOME

ls ~/.dart_install/lib/$python_dist_dir
