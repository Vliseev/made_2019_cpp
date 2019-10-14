cmake_minimum_required(VERSION 3.11)
project(calc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -O0 -fsanitize=address")

add_executable(calc main.cpp Lexer.cpp Interpreter.cpp)
