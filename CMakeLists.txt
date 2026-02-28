cmake_minimum_required(VERSION 3.28)
project(main)
add_executable(main main.cpp src/function.cpp)
target_include_directories(main PRIVATE include)