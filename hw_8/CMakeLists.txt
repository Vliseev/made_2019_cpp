cmake_minimum_required(VERSION 3.10)
project(8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

add_executable(8 main.cpp )