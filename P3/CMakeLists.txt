cmake_minimum_required(VERSION 3.12)
project(MyProject)

include_directories( . )

set(CMAKE_CXX_STANDARD 11) 


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_executable (test_heap test_heap.cpp)
