## Installation robot_dart

### Dependencies

#### Required
- Ubuntu (it should work on versions >= 14.04) or OSX
- Eigen3 (needed by DART)
- Boost (needed by DART)
- DART, http://dartsim.github.io/

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

For **Ubuntu systems**, please follow the detailed installation instructions on the [DART documentation website](http://dartsim.github.io/install_dart_on_ubuntu.html#install-required-dependencies). Make sure that you don't forget to add the PPAs as detailed [here](http://dartsim.github.io/install_dart_on_ubuntu.html#adding-personal-package-archives-ppas-for-dart-and-dependencies). What is more, you need to enable the `-DART_ENABLE_SIMD` flag in the CMake configuration. In addition, you need the following optional dependencies: **DART Parsers** and **OpenSceneGraph GUI**. Lastly, it is recommended to use either the `master` or `release-6.7` branch (and not the one provided in DART's documentation). In short you should do the following:

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
sudo apt-get install libopenscenegraph-dev

sudo apt-get install libtinyxml-dev libtinyxml2-dev
sudo apt-get install liburdfdom-dev liburdfdom-headers-dev

cd /path/to/tmp/folder
git clone git://github.com/dartsim/dart.git
cd dart
git checkout release-6.7

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
brew install open-scene-graph

brew install tinyxml
brew install tinyxml2
brew install urdfdom

cd /path/to/tmp/folder
git clone git://github.com/dartsim/dart.git
cd dart
git checkout release-6.7

mkdir build
cd build
cmake -DDART_ENABLE_SIMD=ON ..
make -j4
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

By default the library will be installed in `/usr/lib` (for this maybe `sudo ./waf install` might be needed) and a static library will be generated. You can change the defaults as follows:

- `./waf configure --prefix=/path/to/install/dir --shared`
- `./waf install`

In short, with `--prefix` you can change the directory where the library will be installed and if `--shared` is present a shared library will be created instead of a static one.
