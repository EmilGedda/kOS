
add_library(kernel STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/mem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/idt.cpp"
    )
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/io")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/string")

target_link_libraries(kernel PRIVATE io)
