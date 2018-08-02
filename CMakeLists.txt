cmake_minimum_required(VERSION 3.10)
project(kOS C CXX)

set(CMAKE_CXX_FLAGS "--target=x86_64-pc-none-elf -std=c++17 -march=native -Os -Wall -Wextra -Weffc++ -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-rtti -mno-sse -g")

# libkernel
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

# kmain
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/boot")

add_custom_command(
    OUTPUT "kos.elf"
    COMMAND ${CMAKE_LINKER} -o kos.elf -n -T${LINKER_FILE} $<TARGET_FILE:boot> $<TARGET_FILE:kmain>
    DEPENDS kmain boot
)

add_custom_target(kos_elf ALL DEPENDS "kos.elf")

add_custom_command(
    OUTPUT "kos.iso"
    COMMAND cp -f kos.elf ../iso/boot/kos.elf
    COMMAND grub-mkrescue -o kos.iso ../iso
    DEPENDS "kos.elf"
)

add_custom_target(kos_iso ALL DEPENDS "kos.iso" )
