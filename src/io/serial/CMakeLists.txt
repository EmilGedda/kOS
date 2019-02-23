add_kernel_library(serial EXCLUDE_FROM_ALL
    "${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp"
    )

target_compile_options(serial PUBLIC ${KERNEL_FLAGS})
target_compile_options(serial PUBLIC -D__ELF__ -include ${LIBCXX_CONFIG}) # TODO: fix
target_include_directories(serial PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../../include")
