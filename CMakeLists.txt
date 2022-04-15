cmake_minimum_required(VERSION 3.22)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "-g")

find_package(SDL2 REQUIRED)

project(nodebuilder)

add_executable(nodebuilder
    src/main.cpp
    src/map.cpp
    src/renderer.cpp
    src/splitter.cpp
    src/wad.cpp)
    
target_link_libraries(nodebuilder PRIVATE SDL2::SDL2)
target_compile_features(nodebuilder PRIVATE cxx_std_17)
