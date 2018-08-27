add_library(stdlib INTERFACE)
target_compile_features(stdlib INTERFACE cxx_std_20)
target_include_directories(stdlib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
