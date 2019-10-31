cmake_minimum_required(VERSION 3.10)
project(calc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -O2 -fsanitize=address -fsanitize=undefined")

add_executable(tests test.cpp linear_allocator.cpp)
