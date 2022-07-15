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
In this page we provide standalone scripts for installing RobotDART for `Ubuntu` (20.04) and `OSX`. The scripts will install all the required dependencies and RobotDART. Notably, all dependencies that need to be compiled by source and RobotDART will be installed inside the `/opt` folder. This way, one can be rest assured that their system will be clean.

### Ubuntu 20.04

To quickly install RobotDART on `Ubuntu 20.04`, we just need to run `./scripts/install_ubuntu.sh` from the root of the repo. In more detail:

- `git clone https://github.com/resibots/robot_dart.git`
- `cd robot_dart`
- `./scripts/install_ubuntu.sh`

This will install everything needed! Once the script is successfully executed, one should add the following to their `~/.bashrc` or `~/.zshrc` file:

```bash
export PATH=/opt/magnum/bin:$PATH
export LD_LIBRARY_PATH=/opt/dart/lib:/opt/magnum/lib:/opt/robot_dart/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/opt/dart/lib/python3/dist-packages:/opt/robot_dart/lib/python3.8/site-packages:$PYTHONPATH
```

### OSX

**Coming soon**
