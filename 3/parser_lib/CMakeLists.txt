cmake_minimum_required(VERSION 3.14)
project(parser_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fsanitize=undefined")

add_library(parser_lib Parser.cpp)