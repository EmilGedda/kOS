add_executable(kboot bootloader.cpp efi.cpp)
target_link_libraries(kboot uefi)

target_include_directories(kboot PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_compile_options(kboot PRIVATE
    -target x86_64-pc-win32-coff
    -fno-stack-protector
    -fshort-wchar
    -mno-red-zone)

set(CMAKE_CXX_LINK_EXECUTABLE "lld-link <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -out:<TARGET> <LINK_LIBRARIES>")

target_link_options(kboot PRIVATE
    -subsystem:efi_application
    -nodefaultlib
    -dll
    -entry:efi_main)

set_target_properties(kboot
    PROPERTIES 
        OUTPUT_NAME "BOOTX64.EFI"
        CXX_STANDARD 17)
