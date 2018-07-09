FROM ubuntu:bionic

RUN     apt-get update &&   \
        apt-get -y install  \
        cmake               \
        ninja-build         \
        clang               \
        llvm                \
        qemu-system-x86     \
        gcc-multilib

COPY    . /kos

RUN     cd /kos/build               && \
        rm -rf /kos/build/*         && \
        CC=clang cmake -G Ninja ..  && \
        cmake --build .

EXPOSE 5900
CMD     qemu-system-i386 -vga std -nographic -vnc :0 -kernel /kos/build/kos.elf
