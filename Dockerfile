FROM ubuntu:15.04

MAINTAINER "Miguel Flores Ruiz de Eguino (@miguelfrde)"

WORKDIR "/osdev"

# Download package lists
RUN apt-get update

# Install building tools
RUN apt-get -y install build-essential

# Install debugging tools
RUN apt-get -y install libc6-dbg gdb valgrind
