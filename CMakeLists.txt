cmake_minimum_required(VERSION 3.6)
project(plusiki)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp future.h shared_state.h promise.h thread_pool.cpp thread_pool.h map.h flatten.h flattenTuple.h)
add_executable(plusiki ${SOURCE_FILES})