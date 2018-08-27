cmake_minimum_required(VERSION 3.10)
project(kOS C CXX)

set(KERNEL_FLAGS --target=x86_64-pc-none-elf -std=c++2a -march=native -Os -Wall -Wextra -Weffc++ -ffreestanding -nostdlib -fno-exceptions -fno-rtti -mno-sse -g)

# stdlib
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/std")

# libkernel
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include") # TODO: fix this
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

# kmain
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/boot")

# tests
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")

add_custom_command(
    OUTPUT "kos.elf"
    COMMAND ${CMAKE_LINKER} -o kos.elf -n -T${LINKER_FILE} $<TARGET_FILE:boot> $<TARGET_FILE:kmain> $<TARGET_FILE:kernel>
    DEPENDS kmain boot kernel
)

add_custom_target(kos_elf DEPENDS "kos.elf")

add_custom_command(
    OUTPUT "kos.iso"
    COMMAND cp -f kos.elf ../iso/boot/kos.elf
    COMMAND grub-mkrescue -o kos.iso ../iso
    DEPENDS "kos.elf"
)

add_custom_target(kos_iso ALL DEPENDS "kos.iso" )
