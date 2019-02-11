cmake_minimum_required(VERSION 3.10)
project(kOS C CXX)



#add_definitions("-include ${CMAKE_CURRENT_BINARY_DIR}/third_party/libcxx/__generated_config -D__ELF__")

set(LLVM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/llvm" CACHE STRING "Path to llvm")
set(LLVM_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/llvm/include" CACHE STRING "Path to llvm/include")
set(LIBCXX_CXX_ABI "libcxxabi" CACHE STRING "ABI library for libc++")
set(LIBCXX_CXX_ABI_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libcxxabi/include" CACHE STRING "ABI library for libc++")
set(LIBCXXABI_LIBCXX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libcxx/" CACHE STRING "Path to libc++")

#option(LIBCXX_ENABLE_STATIC_ABI_LIBRARY "Statically link the ABI library" ON)
option(LIBCXX_ENABLE_SHARED "Build libc++ as a shared library." OFF)
option(LIBCXX_ENABLE_EXPERIMENTAL_LIBRARY "Build libc++experimental.a" OFF)
option(LIBCXX_INCLUDE_BENCHMARKS "Build the libc++ benchmarks and their dependancies" OFF)
option(LIBCXX_ABI_FORCE_ITANIUM "Ignore auto-detection and force use of the Itanium ABI." ON)


option(LIBCXXABI_ENABLE_ASSERTIONS "Enable assertions independent of build mode." OFF)
option(LIBCXXABI_USE_LLVM_UNWINDER "Build and use the LLVM unwinder." ON)
#option(LIBCXXABI_ENABLE_STATIC_UNWINDER "Statically link the LLVM unwinder." ON)
option(LIBCXXABI_USE_COMPILER_RT "Use compiler-rt instead of libgcc" ON)
option(LIBCXX_USE_COMPILER_RT "Use compiler-rt instead of libgcc" ON)
option(LIBCXXABI_ENABLE_THREADS "Build with threads enabled" OFF)
option(LIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS
    "Build libc++abi with definitions for operator new/delete. Normally libc++
    provides these definitions" OFF)
option(LIBCXXABI_ENABLE_SHARED "Build libc++abi as a shared library." OFF)
option(LIBCXXABI_BAREMETAL "Build libc++abi for baremetal targets." ON)
set(LIBCXXABI_TARGET_TRIPLE "x86_64-pc-none-elf" CACHE STRING "Target triple for cross compiling.")
option(LIBCXXABI_SILENT_TERMINATE "Set this to make the terminate handler default to a silent alternative" ON)
option(LIBCXX_ENABLE_STDIN "Build libc++ with support for stdin/std::cin." OFF)
option(LIBCXX_ENABLE_STDOUT "Build libc++ with support for stdout/std::cout." OFF)
option(LIBCXX_ENABLE_THREADS "Build libc++ with support for threads." OFF)
option(LIBCXX_ENABLE_MONOTONIC_CLOCK
  "Build libc++ with support for a monotonic clock.
  This option may only be set to OFF when LIBCXX_ENABLE_THREADS=OFF." OFF)

set(Python_ADDITIONAL_VERSION "2.7" CACHE STRING "")
include(FindPythonInterp)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/libcxx")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/libcxx/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/libcxxabi")

option(LIBUNWIND_ENABLE_ASSERTIONS "Enable assertions independent of build mode." OFF)
option(LIBUNWIND_ENABLE_THREADS "Build libunwind with threading support." OFF)
option(LIBUNWIND_USE_COMPILER_RT "Use compiler-rt instead of libgcc" ON)
option(LIBUNWIND_ENABLE_SHARED "Build libunwind as a shared library." OFF)
set(LIBUNWIND_TARGET_TRIPLE "x86_64-pc-none-elf -DNDEBUG -D_LIBUNWIND_IS_BAREMETAL" CACHE STRING "")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/libunwind")

set(KERNEL_FLAGS --target=x86_64-pc-none-elf -std=c++2a -march=native -Os -Wall -Wextra -Weffc++ -ffreestanding -nostdlib -mno-sse -g)

set(LIBCXX_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/third_party/libcxx/__generated_config")

add_dependencies(unwind_objects cxx-generated-config)
add_dependencies(cxxabi_objects cxx-generated-config)

target_compile_options(cxx_objects PUBLIC -fno-stack-protector -include ${LIBCXX_CONFIG})
target_compile_options(unwind_objects PUBLIC -include ${LIBCXX_CONFIG})
target_compile_options(cxxabi_objects PUBLIC -include ${LIBCXX_CONFIG})

target_compile_definitions(cxx_static PUBLIC -D__ELF__)
target_compile_definitions(cxxabi_objects PRIVATE -D__ELF__)
target_compile_definitions(unwind_objects PRIVATE -D__ELF__)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt")

# libkernel
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_options(kernel PUBLIC -include ${LIBCXX_CONFIG})

# kmain
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/boot")

# tests
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")

add_custom_command(
    OUTPUT "kos.elf"
    COMMAND clang -static ${KERNEL_FLAGS} -o kos.elf -T${LINKER_FILE} -Wl,--start-group $<TARGET_FILE:boot> $<TARGET_FILE:unwind_static> $<TARGET_FILE:cxxabi_static> $<TARGET_FILE:cxx_static> $<TARGET_FILE:kmain> $<TARGET_FILE:kernel> -Wl,--end-group
    DEPENDS kmain boot kernel cxx_static cxxabi_static unwind_static
)

add_custom_target(kos_elf DEPENDS "kos.elf")

add_custom_command(
    OUTPUT "kos.iso"
    COMMAND cp -f kos.elf ../iso/boot/kos.elf
    COMMAND grub-mkrescue -o kos.iso ../iso
    DEPENDS "kos.elf"
)

add_custom_target(kos_iso ALL DEPENDS "kos.iso" )
