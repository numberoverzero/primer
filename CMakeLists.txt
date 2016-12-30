cmake_minimum_required(VERSION 3.6)
project(primer)
include_directories(codegen)
include_directories(lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(
    SOURCE_FILES
    main.cpp
    lib/solver.cpp
    lib/solver.h
    lib/debugging.h
    lib/reverse.h
)
add_executable(primer ${SOURCE_FILES})
add_executable(reverse-gen codegen/uint32_t_reverse.cpp)