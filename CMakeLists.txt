cmake_minimum_required(VERSION 3.6)
project(primer)
include_directories(lib)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(
    SOURCE_FILES
    main.cpp
    lib/lookups.cpp
    lib/bits.cpp

        lib/solver.cpp lib/solver.h lib/constants.h lib/debugging.h)
add_executable(primer ${SOURCE_FILES})
