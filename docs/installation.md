## Installation robot_dart

### Dependencies

#### Required
- Ubuntu (it should work on versions >= 14.04) or OSX
- Eigen3 (needed by DART)
- Boost (needed by DART)
- DART, http://dartsim.github.io/

#### Optional
- Magnum (for graphics), https://github.com/mosra/magnum

### Installation of the dependencies

#### Installing Boost and Eigen3

For Ubuntu-based distributions we should use the following commands to install Eigen3 and Boost:

```bash
sudo apt-get update
sudo apt-get install libeigen3-dev libboost-filesystem-dev libboost-system-dev libboost-regex-dev
```

For OSX with brew:

```bash
brew install eigen3
brew install boost
```

#### Installing DART

Of course in order to use robot\_dart, you need to install [DART](http://dartsim.github.io/) (from source).

For **Ubuntu systems**, please follow the detailed installation instructions on the [DART documentation website](http://dartsim.github.io/install_dart_on_ubuntu.html#install-required-dependencies). Make sure that you don't forget to add the PPAs as detailed [here](http://dartsim.github.io/install_dart_on_ubuntu.html#adding-personal-package-archives-ppas-for-dart-and-dependencies). What is more, you need to enable the `-DART_ENABLE_SIMD` flag in the CMake configuration. In addition, you need the following optional dependency: **DART Parsers**. Lastly, it is recommended to use either the `master` or `release-6.9` branch (and not the one provided in DART's documentation). In short you should do the following:

**Ubuntu <= 14.04 only**

```bash
sudo apt-add-repository ppa:libccd-debs/ppa
sudo apt-add-repository ppa:fcl-debs/ppa
```

**For all Ubuntu distributions**

```bash
sudo apt-add-repository ppa:dartsim/ppa
sudo apt-get update

sudo apt-get install build-essential cmake pkg-config git
sudo apt-get install libeigen3-dev libassimp-dev libccd-dev libfcl-dev libboost-regex-dev libboost-system-dev

sudo apt-get install libtinyxml-dev libtinyxml2-dev
sudo apt-get install liburdfdom-dev liburdfdom-headers-dev

cd /path/to/tmp/folder
git clone git://github.com/dartsim/dart.git
cd dart
git checkout release-6.9

mkdir build
cd build
cmake -DDART_ENABLE_SIMD=ON ..
make -j4
sudo make install
```

If you want to install DART somewhere else than `/usr/local`, you should use the `-DCMAKE_INSTALL_PREFIX=/path/to/install/folder` cmake argument.

For **OSX systems** with homebrew, please follow the detailed installation instructions on the [DART documentation website](http://dartsim.github.io/install_dart_on_mac.html#install-from-source-using-homebrew). You need to follow the same procedure as for Ubuntu systems. In short you should do the following:

```bash
brew install eigen
brew install assimp
brew install libccd
brew install dartsim/dart/fcl04
brew install boost

brew install tinyxml
brew install tinyxml2
brew install urdfdom

cd /path/to/tmp/folder
git clone git://github.com/dartsim/dart.git
cd dart
git checkout release-6.9

mkdir build
cd build
cmake -DDART_ENABLE_SIMD=ON ..
make -j4
sudo make install
```

#### Installing Magnum

Magnum depends on [Corrade](https://github.com/mosra/corrade) and we are going to use a few plugins and extras from the library. We are also going to use Glfw and Glx for the back-end. Follow the instrutions below:

```bash
#installation of Glfw and OpenAL
# Ubuntu
sudo apt-get install libglfw3-dev libglfw3 libopenal-dev libassimp-dev
# Mac OSX
brew install glfw3 openal-soft assimp

# installation of Corrade
cd /path/to/tmp/folder
git clone https://github.com/mosra/corrade.git
cd corrade
mkdir build && cd build
cmake ..
make -j
sudo make install

# installation of Magnum
cd /path/to/tmp/folder
git clone https://github.com/mosra/magnum.git
cd magnum
mkdir build && cd build
# Ubuntu
cmake -DWITH_AUDIO=ON -DWITH_DEBUGTOOLS=ON -DWITH_GL=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_SCENEGRAPH=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_TEXTURETOOLS=ON -DWITH_TRADE=ON -DWITH_GLFWAPPLICATION=ON -DWITH_WINDOWLESSGLXAPPLICATION=ON -DWITH_OPENGLTESTER=ON -DWITH_ANYAUDIOIMPORTER=ON -DWITH_ANYIMAGECONVERTER=ON -DWITH_ANYIMAGEIMPORTER=ON -DWITH_ANYSCENEIMPORTER=ON -DWITH_MAGNUMFONT=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_WAVAUDIOIMPORTER=ON .. # this will enable almost all features of Magnum that are not necessarily needed for robot_dart (please refer to the documentation of Magnum for more details on selecting only the ones that you need)
# Mac OSX
cmake -DWITH_AUDIO=ON -DWITH_DEBUGTOOLS=ON -DWITH_GL=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_SCENEGRAPH=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_TEXTURETOOLS=ON -DWITH_TRADE=ON -DWITH_GLFWAPPLICATION=ON -DWITH_WINDOWLESSCGLAPPLICATION=ON -DWITH_OPENGLTESTER=ON -DWITH_ANYAUDIOIMPORTER=ON -DWITH_ANYIMAGECONVERTER=ON -DWITH_ANYIMAGEIMPORTER=ON -DWITH_ANYSCENEIMPORTER=ON -DWITH_MAGNUMFONT=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_WAVAUDIOIMPORTER=ON .. # this will enable almost all features of Magnum that are not necessarily needed for robot_dart (please refer to the documentation of Magnum for more details on selecting only the ones that you need)
make -j
sudo make install

# installation of Magnum Plugins
cd /path/to/tmp/folder
git clone https://github.com/mosra/magnum-plugins.git
cd magnum-plugins
mkdir build && cd build
cmake -DWITH_ASSIMPIMPORTER=ON -DWITH_DDSIMPORTER=ON -DWITH_JPEGIMPORTER=ON -DWITH_OPENGEXIMPORTER=ON -DWITH_PNGIMPORTER=ON -DWITH_TINYGLTFIMPORTER=ON .. # this will enable quite a few Magnum Plugins that are not necessarily needed for robot_dart (please refer to the documentation of Magnum for more details on selecting only the ones that you need)
make -j
sudo make install

# installation of Magnum DART Integration (DART needs to be installed)
cd /path/to/tmp/folder
git clone https://github.com/mosra/magnum-integration.git
cd magnum-integration
mkdir build && cd build
cmake -DWITH_DART=ON ..
make -j
sudo make install
```

### Compilation and running the examples

The compilation of the library is straight-forward:

- retrieve the code, for instance with `git clone https://github.com/resibots/robot_dart.git`
- `cd /path/to/repo/root`
- `./waf configure`
- `./waf`

Now you can run the examples. For example, to run the arm example you need to type the following: `./build/arm` (or `./build/arm_plain` to run it without graphics).

### Installing the library

To install the library (assuming that you have already compiled it), you need only to run:

- `./waf install`

By default the library will be installed in `/usr/local/lib` (for this maybe `sudo ./waf install` might be needed) and a static library will be generated. You can change the defaults as follows:

- `./waf configure --prefix=/path/to/install/dir --shared`
- `./waf install`

In short, with `--prefix` you can change the directory where the library will be installed and if `--shared` is present a shared library will be created instead of a static one.

### Compiling the python bindings

For the python bindings of robot_dart, we need `numpy` to be installed, `pybind11` and the python bindings of DART (dartpy).

For `numpy` one can install it with `pip` or standard packages. For `pybind11` please follow the instructions on the [dart website](http://dartsim.github.io/install_dartpy_on_ubuntu.html#install-dartpy-from-source) (focus on the pybind11 part, for the other parts follow our instructions above).

For the python bindings of DART, do:

```bash
cd dart

mkdir build_py # we need a different folder
cd build_py
cmake -DDART_BUILD_DARTPY=ON -DDART_ENABLE_SIMD=ON ..
make -j4
sudo make install
```

Then the compilation of robot_dart is almost identical as before:

- retrieve the code, for instance with `git clone https://github.com/resibots/robot_dart.git`
- `cd /path/to/repo/root`
- `./waf configure --python` (`--python` enables the python bindings)
- `./waf`
- Install the library (including the python bindings) as before (no change is needed)
- Depending on your installation directory you might need to update your `PYTHONPATH`, e.g. `export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python3.8/site-packages/`

To run the python examples (for the python examples you need to have enabled the graphics, that is, install Magnum library), run:

- `cd /path/to/repo/root`
- `python src/python/example.py` or `python src/python/example_parallel.py`

#### Common Issues with Python bindings

One of the most common issue with the python bindings is the fact that DART bindings might be compiled and installed for python 3 and the robot_dart ones for python 2. To force the usage of python 3 for robot_dart, you use `python3 ./waf` instead of just `./waf` in all the commands above.
