cmake_minimum_required(VERSION 3.10)  # Use a more recent version of CMake

# Set the project name and version
project(iDom_server-CMAKE VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Specify the include directories
include_directories(json2/)

#check cores
if(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "Ninja")
    execute_process(COMMAND nproc OUTPUT_VARIABLE NUM_CORES)
    string(STRIP ${NUM_CORES} NUM_CORES)
    set(EXTRA_MAKE_OPTIONS "-j${NUM_CORES}")
endif()

# Add link directories
link_directories(/usr/local/lib)

# Set compiler options
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNOSSL")

# Add the library source files
set(LIBS_SRC
    libs/alarm/alarm.cpp
    libs/circular_buffer/circular_buffer.cpp
    libs/config_parser/parser.cpp
    libs/emoji/emoji.cpp
    libs/event_counters/event/event_command.cpp
    libs/event_counters/event/event_mpd.cpp
    libs/event_counters/event/event_pilot.cpp
    libs/event_counters/event/event_unknown.cpp
    libs/event_counters/event/new_connect_event.cpp
    libs/event_counters/event_counters.cpp
    libs/event_counters/event_counters_handler.cpp
    libs/generator/generator.cpp
    libs/sunrise-sunset/sunriseset.cpp
    libs/useful/usefull.cpp
    libs/viberAPI/viber_api.cpp
    libs/ThreadPool/ThreadPool.cpp
)

# Add the source files
set(SOURCE_FILES
    liquidcrystal_i2c-for-ardupi/LCD.cpp
    liquidcrystal_i2c-for-ardupi/I2CIO.cpp
    liquidcrystal_i2c-for-ardupi/LiquidCrystal_I2C.cpp
    mqtt_mosquitto/MQTT_mosquitto/mqtt.cpp
    src/433MHz/rc_433mhz.cpp
    src/433MHz/RFLink/rflinkhandler.cpp
    src/blockQueue/blockqueue.cpp
    src/buderus/buderus.cpp
    src/c_connection/c_connection.cpp
    src/c_connection/c_connection2.cpp
    src/c_connection/c_connection_node.cpp
    src/command/command.cpp
    src/command/commandClass/command_433mhz.cpp
    src/command/commandClass/command_ardu.cpp
    src/command/commandClass/command_big.cpp
    src/command/commandClass/command_buderus.cpp
    src/command/commandClass/command_clock.cpp
    src/command/commandClass/command_cmd.cpp
    src/command/commandClass/command_event.cpp
    src/command/commandClass/command_gateway.cpp
    src/command/commandClass/command_hello.cpp
    src/command/commandClass/command_help.cpp
    src/command/commandClass/command_idom.cpp
    src/command/commandClass/command_recuperator.cpp
    src/command/commandClass/command_light.cpp
    src/command/commandClass/command_log.cpp
    src/command/commandClass/command_mpd.cpp
    src/command/commandClass/command_ok.cpp
    src/command/commandClass/command_program.cpp
    src/command/commandClass/command_put.cpp
    src/command/commandClass/command_shed.cpp
    src/command/commandClass/command_show.cpp
    src/command/commandClass/command_sleep.cpp
    src/command/commandClass/command_state.cpp
    src/command/commandClass/command_uptime.cpp
    src/command/commandClass/command_wifi.cpp
    src/command/commandClass/command_wifi.h
    src/command/commandClass/commandexit.cpp
    src/command/commandClass/commandrs232.cpp
    src/command/commandClass/commandtest.cpp
    src/command/commandhandler.cpp
    src/command/commandhandlergateway.cpp
    src/command/commandhandlermqtt.cpp
    src/command/commandhandlerroot.cpp
    src/command/commandhandlerrs232.cpp
    src/CRON/cron.cpp
    src/functions/functions.cpp
    src/functions/functions2.cpp
    src/functions/mpd_cli.cpp
    src/HOUSE/house_room_handler.cpp
    src/HOUSE/light_bulb.cpp
    src/HOUSE/room.cpp
    src/HTTP/http.cpp
    src/iDom_server_OOP.cpp
    src/iDomKEY_ACCESS/idomkey_access.cpp
    src/iDomSaveState/idom_save_state.cpp
    src/iDomStatus/idomstatus.cpp
    src/iDomTools/idomtools.cpp
    src/iDomTools/idomtools_mpd.cpp
    src/iDomTools/idomtools_system.cpp
    src/LCD_c/lcd_c.cpp
    src/logger/logger.cc
    src/main.cpp
    src/RADIO_433_eq/radio_433_eq.cpp
    src/RADIO_433_eq/radio_button.cpp
    src/SATEL_INTEGRA/satel_integra.cpp
    src/SATEL_INTEGRA/satel_integra_handler.cpp
    src/SerialPi/serialpi.cpp
    src/SerialPi/wiringSerial.c
    src/TASKER/tasker.cpp
    src/THERMOMETER_CONTAINER/thermometer_container.cpp
    src/thread_functions/rs232_thread.h
    src/dbClient/db-client.cpp
    src/dbClient/db-client-factory.cpp
    src/recuperator/recuperator.cpp
)

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

message("dane commit hash :")
message(${GIT_COMMIT_HASH})
message("dane branch :")
message(${GIT_BRANCH})

file(GLOB SOURCE_FILES2 "src/command/*.*"
    "src/command/commandClass/*.*"
    "src/MENU/*.*")

# Define the executable
add_executable(${PROJECT_NAME} ${LIBS_SRC} ${SOURCE_FILES} ${SOURCE_FILES2})

# Link the libraries
target_link_libraries(${PROJECT_NAME}  rt pthread curl mpd mosquitto i2c)
