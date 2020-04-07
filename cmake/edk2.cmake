include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    edk2
    PREFIX ${CMAKE_BINARY_DIR}/edk2
    GIT_REPOSITORY https://github.com/tianocore/edk2.git
    GIT_TAG "edk2-stable202002"
    TIMEOUT 30
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    GIT_SHALLOW 1
    GIT_SUBMODULES ""
)

ExternalProject_Get_Property(edk2 source_dir)
add_library(uefi INTERFACE)

add_dependencies(uefi edk2)
target_include_directories(uefi INTERFACE "${source_dir}/MdePkg/Include"
                                          "${source_dir}/MdePkg/Include/X64")
