cmake_minimum_required(VERSION 2.6)
project(Ex3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES CacheFS.h CacheFS.cpp myFile.cpp myFile.h Block.h Block.cpp Algorithm.h LRUAlg.h LFUAlg.h FBRAlg.h Test3.cpp)
add_executable(Ex3 ${SOURCE_FILES})