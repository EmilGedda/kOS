# additional target to perform clang-format run, requires clang-format


file(GLOB_RECURSE ALL_SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/**/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/**/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../include/**/*.hpp
	)
find_program(CLANG_FORMAT NAMES "clang-format")
if(NOT CLANG_FORMAT)
    message(STATUS "clang-format not found")
else()
    message(STATUS "clang-format found: ${CLANG_FORMAT}")
    add_custom_target(
        format
        COMMAND /usr/bin/clang-format
        -style=file
        -i
        ${ALL_SOURCE_FILES}
    )
endif()
