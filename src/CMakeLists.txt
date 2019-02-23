
add_kernel_library(kernel STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/mem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/idt.cpp"
    )
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/io")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/string")

target_include_directories(kernel PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_compile_options(kernel PUBLIC ${KERNEL_FLAGS})
target_link_libraries(kernel PUBLIC io cxx_static cxxabi_static unwind_static)
