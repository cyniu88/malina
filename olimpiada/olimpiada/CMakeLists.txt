cmake_minimum_required(VERSION 3.5)
project(olimpiada LANGUAGES CXX)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a  -DNOSSL -O3 ")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(olimpiada main.cpp)
target_link_libraries(olimpiada  )
install(TARGETS olimpiada
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

