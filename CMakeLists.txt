cmake_minimum_required(VERSION 3.28)
project(main)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(main main.cpp src/function.cpp)