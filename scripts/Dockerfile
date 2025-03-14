ARG BASE_TAG=24.04
FROM ubuntu:${BASE_TAG} AS base

ARG USER=ubuntu
ENV DEBIAN_FRONTEND=noninteractive \
    USER=$USER \
    HOME=/home/$USER

RUN apt-get update && apt install -y \
    ssh \
    sudo 

RUN echo "Set disable_coredump false" >> /etc/sudo.conf

RUN usermod -a -G dialout $USER
RUN echo "$USER ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/99_aptget
RUN chmod 0440 /etc/sudoers.d/99_aptget && chown root:root /etc/sudoers.d/99_aptget

USER $USER
WORKDIR ${HOME}/robot_dart

COPY --chown=$USER:$USER . .
RUN sudo apt update
RUN /bin/bash ./scripts/install_ubuntu.sh
RUN sudo apt-get clean \
    && sudo rm -rf /var/lib/apt/lists/*
RUN sudo rm -r ${HOME}/robot_dart/temp_robot_dart
RUN chown -R $USER:$USER ${HOME}/robot_dart/waf
RUN chown -R $USER:$USER ${HOME}/robot_dart/wscript

RUN <<EOF 
echo "export PATH=/opt/magnum/bin:$PATH" >> ~/.bashrc
echo "export CMAKE_PREFIX_PATH=/opt/robot_dart:/opt/magnum" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=/opt/magnum/lib:$LD_LIBRARY_PATH" >> ~/.bashrc
echo "export PYTHONPATH=/opt/magnum/lib/python3/site-packages:$PYTHONPATH" >> ~/.bashrc
EOF