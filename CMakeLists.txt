cmake_minimum_required(VERSION 3.6)
project(primer)
include_directories(codegen)
include_directories(lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(SOURCE_FILES
        lib/bits.h
        lib/debugging.h
        lib/reverse.h
        lib/solver.cpp
        lib/solver.h
)
add_executable(primer applications/primer.cpp ${SOURCE_FILES})
add_executable(build-lookup applications/build_lookup.cpp ${SOURCE_FILES})