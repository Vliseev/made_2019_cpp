cmake_minimum_required(VERSION 3.10)

project(bigint)
set(TEST "TRUE")

if (${TEST} STREQUAL "TRUE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  --coverage -g -O0 -fsanitize=address")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O2")
endif ()

set(CMAKE_CXX_STANDARD 17)

add_executable(bigint main.cpp Bigint.cpp Bigint.h)
add_executable(big big.cpp)