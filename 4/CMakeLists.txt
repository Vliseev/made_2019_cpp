cmake_minimum_required(VERSION 3.10)
project(4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")

add_executable(4 main.cpp Matrix.cpp)