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

git clone https://github.com/dartsim/dart.git
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

sudo ldconfig
cd $CI_HOME

# Hack for dartpy installation
python_file=/usr/lib/python3/dist-packages/dartpy.cpython-310-x86_64-linux-gnu.so
if [ -f "$python_file" ]; then
cp $python_file ~/.dart_install/$python_dist_dir
fi
