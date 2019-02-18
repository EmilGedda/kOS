FROM archlinux/base:latest as dev

MAINTAINER Emil Gedda

RUN pacman -Syu --noconfirm \
        cmake               \
        clang               \
        git                 \
        make                \
        binutils            \
        gcc                 \
        nasm                \
        grub                \
        mtools              \
        python              \
        libisoburn

WORKDIR /root
ENTRYPOINT /bin/bash


FROM dev as build
WORKDIR /kos
COPY . /kos
RUN mkdir build \
        && cd build \
        && CC=clang CXX=clang++ cmake .. \
        && make cxx-generated-config \
        && make kos_iso
ENTRYPOINT /bin/bash


FROM alpine:latest as run
RUN apk update && apk add qemu-system-x86_64 qemu-ui-curses
COPY --from=build /kos/build/kos.iso /kos.iso
WORKDIR /
ENTRYPOINT qemu-system-x86_64 -nographic -display curses -cdrom kos.iso
