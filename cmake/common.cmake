include(format-flags.cmake)

find_program(CLANG_TIDY NAMES "clang-tidy")
if(NOT CLANG_TIDY)
    message(WARNING "clang-tidy not found!")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY}")
endif()

function(add_kernel_library name)
    add_library(${name} ${ARGN})
    message(STATUS "Enabling kernel library: ${name}")

    if(CLANG_TIDY) 
	set(FLAGS "-checks=*"
		  "-clang-analyzer-alpha.*"
		  "-warnings-as-errors=*"
		  "-hicpp-no-assembler"
		  "-hicpp-signed-bitwise"
		  "-clang-diagnostic*"
		  "-misc-unused*"
		  "-hicpp-braces-around-statements"
		  "-readability-braces-around-statements"
		  "-readability-inconsistent-declaration-parameter-name"
		  "-readability-implicit-bool-conversion"
		  "-cppcoreguidelines-pro-type-reinterpret-cast"
		  "-cppcoreguidelines-pro-bounds-array-to-pointer-decay")
	string(REPLACE ";" "," CLANG_TIDY_FLAGS "${FLAGS}")
        set(DO_CLANG_TIDY "${CLANG_TIDY}" "${CLANG_TIDY_FLAGS}")
        set_target_properties(
            ${name} PROPERTIES
            CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
    endif()

    add_dependencies(${name} stdlib)
    target_link_libraries(${name} PRIVATE type_safe debug_assert)
    
endfunction()
