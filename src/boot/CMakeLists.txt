enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(LINKER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld" CACHE STRING "The linker-file specifying the ELF layout")

add_library(kmain STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp"
)

target_compile_options(kmain PUBLIC ${KERNEL_FLAGS})
target_link_libraries(kmain PUBLIC kernel)

add_library(boot 
    "${CMAKE_CURRENT_SOURCE_DIR}/boot.asm"
    "${CMAKE_CURRENT_SOURCE_DIR}/longmode.asm"
) 
