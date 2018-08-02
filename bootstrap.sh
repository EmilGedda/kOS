cd build
CC=clang cmake ..
cmake --build .
qemu-system-i386 -kernel kos.elf 2>&1 >/dev/null &
vncviewer ::1:5900
