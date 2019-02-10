add_library(vga EXCLUDE_FROM_ALL
    "${CMAKE_CURRENT_SOURCE_DIR}/buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cursor.cpp"
    )

target_compile_options(vga PUBLIC ${KERNEL_FLAGS})
target_compile_options(vga PUBLIC -D__ELF__ -include ${LIBCXX_CONFIG}) # TODO: fix
target_include_directories(vga PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../../include")
