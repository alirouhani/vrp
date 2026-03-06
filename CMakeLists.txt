cmake_minimum_required(VERSION 3.22.1)
project(vrp_project)

# 1. Project Settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Find Installed Packages
find_package(ortools REQUIRED)

# 3. Handle Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# 4. Main Executable
add_executable(main main.cpp src/Vrp_Solver.cpp)

# Link the imported target from find_package
target_link_libraries(main PRIVATE ortools::ortools)

# Use target-specific include directories instead of global ones
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 5. Testing
enable_testing()

# Test executable
add_executable(unitTest tests/unitTest.cpp src/Vrp_Solver.cpp)
target_link_libraries(unitTest GTest::gtest_main ortools::ortools)

# Discover tests
include(GoogleTest)
gtest_discover_tests(unitTest)