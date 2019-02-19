# kOS - A cozy OS
kOS is a 64bit operating system for the x86_64 ISA. kOS is a microkernel operating system indended for educational purposes to learn and easily grasp some of the core concepts in operating systems such as memory allocation and paging, scheduling, and multithreading. Kos is Norwegian for cozy.

## Features

* Written in C++20, utilising strong typing and modern features to minimize bugs
* GRUB2 bootloader for a .iso to distribute
* Easy building and testing through Docker
* Interrupts and exceptions

### Planned features

* Higher half kernel
* Documentation
* Module loading
* Integration and unit testing
* Userspace
* CPU scheduler
* Hotswapping of kernel modules (such as CPU scheduler or memory manager)
* Fast IPC through memory mapping

## Trying it out

kOS.iso can (hopefully) be booted and run on any x86_64 CPU, but for development and testing running it throught QEMU is just fine. To test it out it is easiest to use docker. I've set up a prebuilt docker image with kos.iso and qemu preloaded, to try it out:
```
$ docker pull gedda/kos
$ docker run --rm -t gedda/kos
```
This will start a container and run kOS inside of QEMU. Ctrl+c to exit.

## Building and installing
Docker has been set up as an optional build environment helper, if you don't have the latest and greatest clang at your disposal. If you don't want to use docker, you can generate the build system through CMake (don't forget to force a clang 7 or later compiler, GCC is not supported) and then simply make the kos_iso target, check the Dockerfile for detailed instructions.

The Dockerfile consists of multi-stage builds depending on what you want to do.
To simply set up and enter a build environment you may run this in your terminal:
```
$ docker build -v.:/kos --target dev -t kos:dev . 
$ docker run -it kos:dev
````

To compile once and get the kos.iso out of docker we need to create a temporary container to extract the iso from the image:
```
$ docker build --target build -t kos:build . 
$ CID=$(docker create kos:build)
$ docker cp $CID:/kos/build/kos.iso . 
$ docker rm $CID
````

To build the whole minimal runnable Docker image from scratch:
```
$ docker build -t kos .
$ docker run -it kos
```

#### License

Currently MIT, see LICENSE file.


Copyright Emil Gedda, 2018-2019
