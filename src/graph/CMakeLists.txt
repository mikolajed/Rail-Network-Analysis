#project(Graph)
#set (CMAKE_CXX_STANDARD 17)
set(BUILD_TESTS OFF)

if (BUILD_TESTS)
	add_executable(test graph.cpp tests.cpp)
endif()

add_library(graph graph.cpp)

