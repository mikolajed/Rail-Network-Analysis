#cmake_minimum_required(version 3.11) # fetchcontent is available in 3.11+
project(PRELIMINARY)
set(CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_STANDARD 11)
add_executable(${PROJECT_NAME} main.cpp implementation.cpp)
