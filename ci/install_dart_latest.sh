set -e

# Select Compiler
if [ "$COMPILER" = "gcc" ]; then
export CC=/usr/bin/gcc && export CXX=/usr/bin/g++
fi
if [ "$COMPILER" = "clang" ]; then
export CC=/usr/bin/clang && export CXX=/usr/bin/clang++
fi

PREFIX=/usr
DEPS_FOLDER=~/.deps
DART_TAG=v6.13.1
SUDOCMD=sudo

PREFIX=${1:-$PREFIX}
DEPS_FOLDER=${2:-$DEPS_FOLDER}
DART_TAG=${3:-$DART_TAG}
SUDOCMD=${4:-$SUDOCMD}

eval SUDOCMD="${SUDOCMD}"

python_dist_dir=$(python -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True, prefix=''))")
${SUDOCMD} mkdir -p ${PREFIX}/$python_dist_dir

mkdir -p ${DEPS_FOLDER}
cd ${DEPS_FOLDER}

if [ ! -d "dart" ]
then
git clone https://github.com/dartsim/dart.git
fi
cd dart
git switch -c testbranch $DART_TAG
mkdir -p build
cd build

# Build DART
if [ "$BUILD_PYTHON" = "ON" ]; then
cmake -DDART_TREAT_WARNINGS_AS_ERRORS=OFF -DDART_BUILD_DARTPY=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
else
cmake -DDART_TREAT_WARNINGS_AS_ERRORS=OFF -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
fi
make -j4
${SUDOCMD} make install

# We are inside the CI
if [ "$PREFIX" = "/home/runner/.dart_install" ]; then
sudo ldconfig
cd $CI_HOME

# Hack for dartpy installation
python_file=/usr/lib/python3/dist-packages/dartpy.cpython-310-x86_64-linux-gnu.so
if [ -f "$python_file" ]; then
sudo cp $python_file ${PREFIX}/$python_dist_dir
fi

else
# Hack for dartpy installation
python_dot_version=$(python -c "import sys; print('.'.join(str(s) for s in sys.version_info[:2]))")
python_version=$(python -c "import sys; print(''.join(str(s) for s in sys.version_info[:2]))")
python_global_install=/usr/lib/python${python_dot_version}/site-packages/dartpy.cpython-${python_version}-x86_64-linux-gnu.so
if [ -f "$python_global_install" ]; then
${SUDOCMD} mv $python_global_install ${PREFIX}/$python_dist_dir
fi
fi
