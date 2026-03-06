cmake_minimum_required(VERSION 3.22)
project(vrp_project)

set(CMAKE_CXX_STANDARD 20)

# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------
find_package(ortools REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# ---------------------------------------------------------
# Core library (compiled once)
# ---------------------------------------------------------
add_library(vrp_core OBJECT
    src/Vrp_Solver.cpp
)

target_include_directories(vrp_core PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(vrp_core PUBLIC
    ortools::ortools
)

# ---------------------------------------------------------
# Application
# ---------------------------------------------------------
add_executable(vrp_app
    main.cpp
    $<TARGET_OBJECTS:vrp_core>
)

target_link_libraries(vrp_app PRIVATE
    ortools::ortools
)

# ---------------------------------------------------------
# Tests
# ---------------------------------------------------------
enable_testing()

add_executable(vrp_tests
    tests/solver_test.cpp
    $<TARGET_OBJECTS:vrp_core>
)

target_link_libraries(vrp_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    ortools::ortools
)

include(GoogleTest)
gtest_discover_tests(vrp_tests)