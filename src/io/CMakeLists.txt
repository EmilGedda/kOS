add_library(io STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/io.cpp"    
)

add_subdirectory(vga)
add_subdirectory(serial)

target_link_libraries(io PUBLIC vga)
target_link_libraries(io PUBLIC serial)
