add_library(io INTERFACE)

add_subdirectory(vga)

target_link_libraries(io INTERFACE vga)
