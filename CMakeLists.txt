cmake_minimum_required(VERSION 3.10)
project(kOS C CXX)

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CLANG_CROSSCOMPILE_FLAGS --target=x86_64-pc-none-elf)
set(CLANG_KERNEL_FLAGS ${CLANG_CROSSCOMPILE_FLAGS} -std=c++17 -march=native -Og -Wall -Wextra -Weffc++ -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-rtti -g)
set(CLANG_ELF_LINKER_FLAGS "${CLANG_CROSSCOMPILE_FLAGS} -std=c99 -ffreestanding -nostdlib -g")
set(LINKER_FILE "${CMAKE_SOURCE_DIR}/src/boot/linker.ld")

add_library(boot OBJECT 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/boot/boot.asm"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/boot/longjmp.asm"
) 
add_library(kmain 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/boot/kmain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io/vga.cpp"
)

target_include_directories(kmain PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(kmain BEFORE PRIVATE ${CLANG_KERNEL_FLAGS})

add_executable(kos_elf $<TARGET_OBJECTS:boot>)
set(CMAKE_LINKER /usr/bin/ld)
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")

target_link_libraries(kos_elf PUBLIC kmain)
set_target_properties(kos_elf PROPERTIES 
    LINK_FLAGS "-n -T${LINKER_FILE}"
    OUTPUT_NAME "kos.elf"
    LINKER_LANGUAGE C
)

add_custom_command(
    OUTPUT "kos.iso"
    COMMAND cp $<TARGET_FILE:kos_elf> ../iso/boot
    COMMAND grub-mkrescue -o kos.iso ../iso
    COMMAND rm -f ../iso/boot/$<TARGET_FILE:kos_elf>
    DEPENDS kos_elf
)

add_custom_target(kos_iso ALL DEPENDS "kos.iso")
