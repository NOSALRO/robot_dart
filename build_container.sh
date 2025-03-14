#!/bin/bash

VM_NAME=robot-dart

HELP_MESSAGE="Usage: build.sh [options]
Options:
  --use-anka               (For darwin systems) use anka to containerize the robot dart.

  --use-novnc              Build container with novnc for systems where x11 forwarding or opengl might not be possible (e.g., osx).

  -h|--help                Show this help message.
"

USE_ANKA=false
USE_NOVNC=false
while [ "$#" -gt 0 ]; do
  case "$1" in
    --use-anka) USE_ANKA=true; shift 1;;
    --use-novnc) USE_NOVNC=true; shift 1;;
    -h|--help) echo "$HELP_MESSAGE"; exit 0;;
    -*) echo "Unknown option: $1" >&2; echo "$HELP_MESSAGE"; exit 1;;
  esac
done


if [[ "$OSTYPE" == "darwin"* && "$USE_ANKA" == "true" ]]; then
    anka create --ram-size 8G --cpu-count 4 --disk-size 30G $VM_NAME latest
    anka start $VM_NAME
    anka run $VM_NAME bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    anka cp -R . $VM_NAME:/Users/anka/robot_dart
    anka run $VM_NAME bash -c "eval \"$(/opt/homebrew/bin/brew shellenv)\" && cd /Users/anka/robot_dart && bash ./scripts/install_osx.sh"
    anka run $VM_NAME bash -c "rm -rf /Users/anka/robot_dart/temp_robot_dart"
    anka stop $VM_NAME
elif [[ "$USE_NOVNC" == "true" ]]; then
    docker build -f scripts/Dockerfile.novnc . --tag $VM_NAME
elif [[ "$OSTYPE" == "linux-gnu"* || "$OSTYPE" == "darwin"* ]]; then
    docker build -f scripts/Dockerfile . --tag $VM_NAME
else
    echo "Unknown system: $OSTYPE"
fi
