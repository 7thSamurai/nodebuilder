cmake_minimum_required(VERSION 3.8)

project(nodebuilder)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

enable_testing()

include_directories(src)
add_subdirectory(src)
add_subdirectory(tests)
