cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -DBT_TEST -DNOSSL -O3")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(koko)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(${PROJECT_NAME} "main.cpp")
