cmake_minimum_required(VERSION 2.8)

project(reload)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DBT_TEST -DNOSSL")
add_executable(${PROJECT_NAME} "main.cpp")
