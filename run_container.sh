#!/bin/bash

CONTAINER_NAME=robot-dart

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

if docker ps --filter "name=$CONTAINER_NAME" --filter "status=running" --format "{{.Names}}" | grep -q "^$CONTAINER_NAME$"; then
    docker exec -it $CONTAINER_NAME bash
else
    if [[ "$OSTYPE" == "darwin"* && "$USE_ANKA" == "true" ]]; then
        VM_NAME=robot-dart
        anka start $VM_NAME
        IP=$(anka show $VM_NAME ip)
        sshpass -p 'admin' ssh anka@$IP
        anka stop $VM_NAME
    else
        IMAGE=docker.io/library/robot-dart

        OS_FLAGS=()

        if [[ "$USE_NOVNC" == "true" ]]; then
            OS_FLAGS+=("-p 8080:8080")
            OS_FLAGS+=("-p 5901:5901")
            OS_FLAGS+=("-p 6080:6080")
        fi
        
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
            xhost +
            OS_FLAGS+=("--privileged")
            OS_FLAGS+=("-e DISPLAY=${DISPLAY}")
            OS_FLAGS+=("-e XAUTHORITY=${XAUTHORITY}")
            OS_FLAGS+=("-v /tmp/.X11-unix:/tmp/.X11-unix:rw")
            OS_FLAGS+=("--device /dev/dri:/dev/dri")
            OS_FLAGS+=("--net host")
        fi
        
        docker run --rm -it \
            --name "${CONTAINER_NAME}" \
            -v .:/home/ubuntu/robot_dart:rw \
            ${OS_FLAGS[@]} \
            -t $IMAGE
    fi
fi