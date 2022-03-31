cmake_minimum_required(VERSION 3.22)

project(nodebuilder)

add_executable(nodebuilder
    src/main.cpp
    src/wad.cpp)

target_compile_features(nodebuilder PRIVATE cxx_std_17)
