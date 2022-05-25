#cmake_minimum_required(version 3.11) # fetchcontent is available in 3.11+
project(PROJECT)
set(CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_STANDARD 11)
add_executable(${PROJECT_NAME} main.cpp implementation.cpp)
add_executable("TEST" test.cpp implementation)
