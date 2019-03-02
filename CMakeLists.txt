cmake_minimum_required(VERSION 3.0)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project(utils)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)


add_executable(ascii2bin src/ascii2bin.c)
add_executable(hex src/hex.c)
add_executable(parsefloat32 src/parsefloat32.cpp)
add_executable(rm_cr src/rm_cr.c)
