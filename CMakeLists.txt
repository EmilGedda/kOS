cmake_minimum_required(VERSION 3.10)
project(kOS)

set(CLANG_CROSSCOMPILE_FLAGS --target=i686-pc-none-elf -march=i686)
set(CLANG_KERNEL_FLAGS ${CLANG_CROSSCOMPILE_FLAGS} -std=c99 -ffreestanding -nostdlib -g)
set(CLANG_ELF_LINKER_FLAGS ${CLANG_CROSSCOMPILE_FLAGS} -m32 -std=c99 -ffreestanding -nostdlib -g -lgcc)
set(LINKER_FILE "${CMAKE_CURRENT_SOURCE_DIR/src/boot/linker.ld}")

add_library(boot OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/src/boot/boot.s")
set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/boot/boot.s" PROPERTY LANGUAGE C)

target_compile_options(boot BEFORE PRIVATE "${CLANG_CROSSCOMPILE_FLAGS}")
add_library(kernel OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/src/boot/kernel.c")
target_compile_options(kernel BEFORE PRIVATE "${CLANG_KERNEL_FLAGS}")


add_executable(kos_elf $<TARGET_OBJECTS:boot> $<TARGET_OBJECTS:kernel>)
set_target_properties(kos_elf PROPERTIES 
    LINK_DEPENDS "${LINKER_FILE}"
    OUTPUT_NAME "kos.elf"
)
target_link_libraries(kos_elf ${CLANG_ELF_LINKER_FLAGS})
