cmake_minimum_required(VERSION 3.14)
project(3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")

add_executable(3 main.cpp Parser.cpp Parser.h likely.h)
target_link_libraries(3 fmt)