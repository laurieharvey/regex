cmake_minimum_required(VERSION 3.0.0)
project(regex VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

option(BUILD_TESTS "Build regex tests" ON)
option(BUILD_SHARED_LIBS "Building shared libraries" OFF)

configure_file(include/regex.h.in regex.h)

add_subdirectory(include)
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(googletest)
    add_subdirectory(benchmark)
    add_subdirectory(test)
endif ()