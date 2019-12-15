cmake_minimum_required(VERSION 3.11)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(thread_pool main.cpp)
target_link_libraries(thread_pool pthread)
