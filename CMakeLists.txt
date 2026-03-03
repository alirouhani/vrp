cmake_minimum_required(VERSION 3.22.1)
project(vrp_project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Main executable
add_executable(main main.cpp src/Params.cpp)

# Enable testing
enable_testing()

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Test executable
add_executable(unitTest tests/unitTest.cpp src/Params.cpp)
target_link_libraries(unitTest GTest::gtest_main)

# Discover tests
include(GoogleTest)
gtest_discover_tests(unitTest)