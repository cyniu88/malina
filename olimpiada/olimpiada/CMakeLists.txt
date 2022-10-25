cmake_minimum_required(VERSION 3.5)

project(olimpiada LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(olimpiada main.cpp)

install(TARGETS olimpiada
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
