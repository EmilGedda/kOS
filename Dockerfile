FROM archlinux/base:latest as dev

MAINTAINER Emil Gedda

RUN pacman -Syu --noconfirm \
        openssh             \
        cmake               \
        clang               \
        git                 \
        ninja               \
        binutils            \
        gcc                 \
        nasm                \
        grub                \
        mtools              \
        python              \
        libisoburn

ENV CC clang
ENV CXX clang++
WORKDIR /kos
ENTRYPOINT /bin/bash


FROM dev as build
WORKDIR /kos
COPY . /kos
RUN mkdir build \
        && cd build \
        && cmake -G Ninja .. \
        && ninja iso | tr -d '\r'
ENTRYPOINT /bin/bash


FROM alpine:latest as run
RUN apk update && apk add qemu-system-x86_64 qemu-ui-curses
COPY --from=build /kos/build/kos.iso /kos.iso
WORKDIR /
ENTRYPOINT qemu-system-x86_64 -nographic -display curses -cdrom kos.iso
