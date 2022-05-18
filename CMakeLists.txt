cmake_minimum_required(version 3.11) # fetchcontent is available in 3.11+
project(rails)
set (cmake_cxx_standard 17)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(src/graph)
target_include_directories(${PROJECT_NAME} PUBLIC src/graph)
target_link_directories(${PROJECT_NAME} PRIVATE src/graph)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json graph)
