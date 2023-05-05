cmake_minimum_required(VERSION 3.25)
project(PIAA2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,500000000")

add_executable(PIAA2 main.cpp)
