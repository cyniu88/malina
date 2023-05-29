cmake_minimum_required(VERSION 3.5)
project(olimpiada LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17  -DNOSSL -O2  ")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_link_options(-rdynamic)

add_executable(olimpiada main.cpp)
target_link_libraries(olimpiada  )
install(TARGETS olimpiada
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

