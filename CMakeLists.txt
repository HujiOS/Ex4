cmake_minimum_required(VERSION 3.2)
project(Ex3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES CacheFS.h CacheFS.cpp myFile.cpp myFile.h Block.h Block.cpp Algorithm.h)
add_executable(Ex4 ${SOURCE_FILES})