enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

add_library(kmain 
    "${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp"
)

target_compile_options(kmain PRIVATE ${CLANG_KERNEL_FLAGS})
target_link_libraries(kmain PRIVATE kernel)


add_library(boot OBJECT 
    "${CMAKE_CURRENT_SOURCE_DIR}/boot.asm"
    "${CMAKE_CURRENT_SOURCE_DIR}/longjmp.asm"
) 

add_executable(kos_elf $<TARGET_OBJECTS:boot>)

set(CMAKE_LINKER /usr/bin/ld)
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(LINKER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

target_link_libraries(kos_elf PUBLIC kmain)
set_target_properties(kos_elf PROPERTIES 
    LINK_FLAGS "-n -T${LINKER_FILE}"
    OUTPUT_NAME "kos.elf"
    LINKER_LANGUAGE C
)
