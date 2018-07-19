cmake_minimum_required(VERSION 3.10)
project(kOS C CXX)

set(CLANG_CROSSCOMPILE_FLAGS --target=x86_64-pc-none-elf)
set(CLANG_KERNEL_FLAGS ${CLANG_CROSSCOMPILE_FLAGS} -std=c++17 -march=native -Os -Wall -Wextra -Weffc++ -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-rtti -g)


# libkernel
add_library(kernel STATIC)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_options(kernel PRIVATE ${CLANG_KERNEL_FLAGS})
target_include_directories(kernel PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


# kmain
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/boot")

add_custom_command(
    OUTPUT "kos.iso"
    COMMAND cp $<TARGET_FILE:kos_elf> ../iso/boot
    COMMAND grub-mkrescue -o kos.iso ../iso
    COMMAND rm -f ../iso/boot/$<TARGET_FILE:kos_elf>
    DEPENDS kos_elf
)

add_custom_target(kos_iso ALL DEPENDS "kos.iso")
