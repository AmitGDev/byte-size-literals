cmake_minimum_required(VERSION 3.31)

project(byte-size-literals
    VERSION 1.0.0
    DESCRIPTION "byte size literals for C++"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Automatically find all source and header files ----
file(GLOB_RECURSE SOURCES 
    "source/*.cpp"
    "source/*.hpp"
    "source/*.h"
)

# ---- Executable ----
add_executable(byte_size_literals ${SOURCES})

# This tells CMake where to look for your .hpp file
target_include_directories(byte_size_literals PRIVATE source)

# ---- Compiler options ----
if (MSVC)
    target_compile_options(byte_size_literals PRIVATE
        /W4
        /permissive-
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(byte_size_literals PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Release>:-O3>
    )
endif()

# ---- Output directory ----
set_target_properties(byte_size_literals PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ---- Install ----
install(TARGETS byte_size_literals
    RUNTIME DESTINATION bin
)

# ---- Configuration summary ----
message(STATUS "")
message(STATUS "Configuration:")
message(STATUS "  Version:        ${PROJECT_VERSION}")
message(STATUS "  C++ standard:   C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler:       ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Files found:    ${SOURCES}")
message(STATUS "")