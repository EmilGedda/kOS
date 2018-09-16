# Prepare doctest for other targets to use
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/doctest/doctest")
