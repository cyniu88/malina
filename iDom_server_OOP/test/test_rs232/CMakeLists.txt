cmake_minimum_required(VERSION 2.8)

project(test_rs232)
include_directories("/home/pi/programowanie/arduPi/")
set(SOURCE_FILES  "main.cpp"
    "/home/pi/programowanie/arduPi/arduPi.cpp"
    "/home/pi/programowanie/arduPi/arduPi_template.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  pthread gtest curl rt pthread gmock)
