FROM alpine:3.8

MAINTAINER Emil Gedda

RUN apk update && apk add   \
        qemu-system-x86_64  \
        cmake               \
        clang               \
        git                 \
        make                \
        binutils            \
        gcc                 \
        musl-dev            \
        nasm                \
        grub                \
        xorriso

RUN ln -s /usr/lib/libstdc++.so.6 /usr/lib/libstdc++.so
ENTRYPOINT /bin/sh
