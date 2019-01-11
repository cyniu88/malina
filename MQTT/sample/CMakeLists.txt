cmake_minimum_required(VERSION 2.8)

project(sample)
set(SOURCE_FILES  "main.cpp"
    "../raspberry_osio_client/raspberry_osio_client.cpp"
    )
add_executable(${PROJECT_NAME} ${SOURCE_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} mosquitto )
