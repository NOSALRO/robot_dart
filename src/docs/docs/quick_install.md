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

## Scripts for Quick Installation of RobotDART

In this page we provide standalone scripts for installing RobotDART for `Ubuntu` (>=20.04) and `OSX`. The scripts will install all the required dependencies and RobotDART. Notably, all dependencies that need to be compiled by source and RobotDART will be installed inside the `/opt` folder. This way, one can be rest assured that their system will be clean.

#### Video recording to file

[ffmpeg](https://www.ffmpeg.org/) is needed for enabling the video recording capabilities. You can install this at any time using your favorite package manager (e.g. `apt` for Ubuntu or `brew` for Mac OSX).

### Ubuntu >=20.04

To quickly install RobotDART on `Ubuntu >=20.04`, we just need to run `./scripts/install_ubuntu.sh` from the root of the repo. In more detail:

- `git clone https://github.com/resibots/robot_dart.git`
- `cd robot_dart`
- `./scripts/install_ubuntu.sh`

This will install everything needed! Once the script is successfully executed, one should add the following to their `~/.bashrc` or `~/.zshrc` file (you may need to swap the python version to yours[^1]):

```bash
export PATH=/opt/magnum/bin:$PATH
export CMAKE_PREFIX_PATH=/opt/robot_dart:/opt/magnum
export LD_LIBRARY_PATH=/opt/magnum/lib:/opt/robot_dart/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/opt/robot_dart/lib/python3.10/site-packages:$PYTHONPATH
```

<!-- ```bash
export PATH=/opt/magnum/bin:$PATH
export LD_LIBRARY_PATH=/opt/dart/lib:/opt/magnum/lib:/opt/robot_dart/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/opt/dart/lib/python3/dist-packages:/opt/robot_dart/lib/python3.8/site-packages:$PYTHONPATH
``` -->

### OSX

To quickly install RobotDART on `Mac OSX`, we just need to run `./scripts/install_osx.sh` from the root of the repo. In more detail:

- `git clone https://github.com/resibots/robot_dart.git`
- `cd robot_dart`
- `./scripts/install_osx.sh`

This will install everything needed! Once the script is successfully executed, one should add the following to their `~/.bashrc` or `~/.zshrc` file (you may need to swap the python version to yours[^1]):

```bash
export PATH=/opt/magnum/bin:$PATH
export CMAKE_PREFIX_PATH=/opt/robot_dart:/opt/magnum
export LD_LIBRARY_PATH=/opt/magnum/lib:/opt/robot_dart/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/opt/robot_dart/lib/python3.10/site-packages:$PYTHONPATH
```


[^1]: You can run `python --version` to see your version. We only keep the major.minor (ignoring the patch version)
