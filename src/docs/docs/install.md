---
  hide:
    -navigation
---
<style>
  .md-typeset h1,
  .md-content__button {
    display: none;
  }
</style>

## Manual Installation of RobotDART

For the quick installation manual, see the [quick installation page](quick_install.md).

### Dependencies

#### Required
- Ubuntu (it should work on versions >= 14.04) or OSX
- Eigen3
- DART, http://dartsim.github.io/

#### Optional
- Boost (for unit tests)
- Magnum (for graphics), https://github.com/mosra/magnum
- [ffmpeg](https://www.ffmpeg.org/) (for video recording) - please install using your favorite package manager

### Installation of the dependencies

**Note:** The following instructions are high-level and assume people with some experience in building/installing software.

#### Installing system-wide packages

For Ubuntu-based distributions (>=20.04) we should use the following commands:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config git libboost-regex-dev libboost-system-dev libboost-test-dev pybind11-dev
sudo apt-get install libdart-all-dev
```

For OSX with brew:

```bash
brew install dartsim
```

#### Installing Magnum

Magnum depends on [Corrade](https://github.com/mosra/corrade) and we are going to use a few plugins and extras from the library. We are also going to use Glfw and Glx for the back-end. Follow the instrutions below:

```bash
#installation of Glfw and OpenAL
# Ubuntu
sudo apt-get install libglfw3-dev libglfw3 libassimp-dev libopenal-dev libglfw3-dev libsdl2-dev libopenexr-dev libdevil-dev libpng-dev libfaad-dev libfreetype6-dev
# Mac OSX
brew install glfw3 openal-soft assimp

# installation of Corrade
cd /path/to/tmp/folder
git clone https://github.com/mosra/corrade.git
cd corrade
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
sudo make install

# installation of Magnum
cd /path/to/tmp/folder
git clone https://github.com/mosra/magnum.git
cd magnum
mkdir build && cd build
# Ubuntu
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_AUDIO=ON -DWITH_DEBUGTOOLS=ON -DWITH_GL=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_SCENEGRAPH=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_TEXTURETOOLS=ON -DWITH_TRADE=ON -DWITH_GLFWAPPLICATION=ON -DWITH_WINDOWLESSEGLAPPLICATION=ON -DWITH_OPENGLTESTER=ON -DWITH_ANYAUDIOIMPORTER=ON -DWITH_ANYIMAGECONVERTER=ON -DWITH_ANYIMAGEIMPORTER=ON -DWITH_ANYSCENEIMPORTER=ON -DWITH_MAGNUMFONT=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_WAVAUDIOIMPORTER=ON -DTARGET_EGL=ON .. # this will enable almost all features of Magnum that are not necessarily needed for robot_dart (please refer to the documentation of Magnum for more details on selecting only the ones that you need)
# Mac OSX
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_AUDIO=ON -DWITH_DEBUGTOOLS=ON -DWITH_GL=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_SCENEGRAPH=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_TEXTURETOOLS=ON -DWITH_TRADE=ON -DWITH_GLFWAPPLICATION=ON -DWITH_WINDOWLESSCGLAPPLICATION=ON -DWITH_OPENGLTESTER=ON -DWITH_ANYAUDIOIMPORTER=ON -DWITH_ANYIMAGECONVERTER=ON -DWITH_ANYIMAGEIMPORTER=ON -DWITH_ANYSCENEIMPORTER=ON -DWITH_MAGNUMFONT=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_WAVAUDIOIMPORTER=ON .. # this will enable almost all features of Magnum that are not necessarily needed for robot_dart (please refer to the documentation of Magnum for more details on selecting only the ones that you need)
make -j
sudo make install

# installation of Magnum Plugins
cd /path/to/tmp/folder
git clone https://github.com/mosra/magnum-plugins.git
cd magnum-plugins
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_ASSIMPIMPORTER=ON -DWITH_DDSIMPORTER=ON -DWITH_JPEGIMPORTER=ON -DWITH_OPENGEXIMPORTER=ON -DWITH_PNGIMPORTER=ON -DWITH_TINYGLTFIMPORTER=ON -DWITH_STBTRUETYPEFONT=ON .. # this will enable quite a few Magnum Plugins that are not necessarily needed for robot_dart (please refer to the documentation of Magnum for more details on selecting only the ones that you need)
make -j
sudo make install

# installation of Magnum DART Integration (DART needs to be installed) and Eigen Integration
cd /path/to/tmp/folder
git clone https://github.com/mosra/magnum-integration.git
cd magnum-integration
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_DART=ON -DWITH_EIGEN=ON ..
make -j
sudo make install

# Installation of Magnum Python Bindings (some examples use them but this is optional)
cd /path/to/tmp/folder
git clone https://github.com/mosra/magnum-bindings.git
cd magnum-bindings
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DMAGNUM_WITH_PYTHON=ON ..
make -j
cd src/python
sudo python3 setup.py install
```

**For OSX you can also install Magnum via brew:** `brew install --HEAD mosra/magnum/corrade && brew install --HEAD mosra/magnum/magnum && brew install --HEAD mosra/magnum/magnum-plugins && brew install --HEAD mosra/magnum/magnum-integration --with-dartsim`

### Compilation and running the examples

The compilation of the library is straight-forward:

- retrieve the code, for instance with `git clone https://github.com/resibots/robot_dart.git`
- `cd /path/to/repo/root`
- `./waf configure`
- `./waf`

To build the examples, execute this:
`./waf examples`

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

For `numpy` one can install it with `pip` or standard packages. `dartpy` should be installed via the packages above. If not, please see the installation instructions on the main DART website.

Then the compilation of robot_dart is almost identical as before:

- retrieve the code, for instance with `git clone https://github.com/resibots/robot_dart.git`
- `cd /path/to/repo/root`
- `./waf configure --python` (`--python` enables the python bindings)
- `./waf`
- Install the library (including the python bindings) as before (no change is needed)
- Depending on your installation directory you might need to update your `PYTHONPATH`, e.g. `export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python3.10/site-packages/`

To run the python examples (for the python examples you need to have enabled the graphics, that is, install Magnum library), run:

- `cd /path/to/repo/root`
- `python src/python/example.py` or `python src/python/example_parallel.py`

#### Common Issues with Python bindings

One of the most common issue with the python bindings is the fact that DART bindings might be compiled and installed for python 3 and the robot_dart ones for python 2. To force the usage of python 3 for robot_dart, you use `python3 ./waf` instead of just `./waf` in all the commands above.
