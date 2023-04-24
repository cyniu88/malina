cmake_minimum_required(VERSION 2.8)
LINK_DIRECTORIES(/usr/local/lib )

project(iDom_server-CMAKE)

set(INCLUDE_DIRECTORIES "json2/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${INCLUDE_DIRECTORIES})

set(LIBS_SRC
"libs/alarm/alarm.cpp"
"libs/circular_buffer/circular_buffer.cpp"
"libs/config_parser/parser.cpp"
"libs/emoji/emoji.cpp"
"libs/event_counters/event/event_command.cpp"
"libs/event_counters/event/event_mpd.cpp"
"libs/event_counters/event/event_pilot.cpp"
"libs/event_counters/event/event_unknown.cpp"
"libs/event_counters/event/new_connect_event.cpp"
"libs/event_counters/event_counters.cpp"
"libs/event_counters/event_counters_handler.cpp"
"libs/facebookAPI/facebookAPI.cpp"
"libs/generator/generator.cpp"
"libs/sunrise-sunset/sunriseset.cpp"
"libs/useful/usefull.cpp"
"libs/viberAPI/viber_api.cpp"
)
set(SOURCE_FILES
"ardupi/ardupi.cpp"
"ardupi/ardupi_template.cpp"
"ftplibpp/ftplib.cpp"
"liquidcrystal_i2c-for-ardupi/liquidcrystal_i2c.cpp"
"mqtt_mosquitto/mqtt_mosquitto/mqtt.cpp"
"src/433mhz/rc_433mhz.cpp"
"src/433mhz/rflink/rflinkhandler.cpp"
"src/blockqueue/blockqueue.cpp"
"src/buderus/buderus.cpp"
"src/c_connection/c_connection.cpp"
"src/c_connection/c_connection2.cpp"
"src/c_connection/c_connection_node.cpp"
"src/command/command.cpp"
"src/command/commandclass/command_433mhz.cpp"
"src/command/commandclass/command_ardu.cpp"
"src/command/commandclass/command_big.cpp"
"src/command/commandclass/command_buderus.cpp"
"src/command/commandclass/command_clock.cpp"
"src/command/commandclass/command_cmd.cpp"
"src/command/commandclass/command_event.cpp"
"src/command/commandclass/command_gateway.cpp"
"src/command/commandclass/command_hello.cpp"
"src/command/commandclass/command_help.cpp"
"src/command/commandclass/command_idom.cpp"
"src/command/commandclass/command_ip.cpp"
"src/command/commandclass/command_light.cpp"
"src/command/commandclass/command_log.cpp"
"src/command/commandclass/command_mpd.cpp"
"src/command/commandclass/command_ok.cpp"
"src/command/commandclass/command_program.cpp"
"src/command/commandclass/command_put.cpp"
"src/command/commandclass/command_shed.cpp"
"src/command/commandclass/command_show.cpp"
"src/command/commandclass/command_sleep.cpp"
"src/command/commandclass/command_state.cpp"
"src/command/commandclass/command_uptime.cpp"
"src/command/commandclass/command_wifi.cpp"
"src/command/commandclass/command_wifi.h"
"src/command/commandclass/commandexit.cpp"
"src/command/commandclass/commandrs232.cpp"
"src/command/commandclass/commandtest.cpp"
"src/command/commandhandler.cpp"
"src/command/commandhandlergateway.cpp"
"src/command/commandhandlermqtt.cpp"
"src/command/commandhandlerroot.cpp"
"src/command/commandhandlerrs232.cpp"
"src/cron/cron.cpp"
"src/functions/functions.cpp"
"src/functions/functions2.cpp"
"src/functions/mpd_cli.cpp"
"src/house/house_room_handler.cpp"
"src/house/light_bulb.cpp"
"src/house/room.cpp"
"src/http/http.cpp" "src/idom_server_oop.cpp"
"src/idomkey_access/idomkey_access.cpp"
"src/idomsavestate/idom_save_state.cpp"
"src/idomstatus/idomstatus.cpp"
"src/idomtools/idomtools.cpp"
"src/idomtools/idomtools_mpd.cpp"
"src/idomtools/idomtools_system.cpp"
"src/lcd_c/lcd_c.cpp"
"src/logger/logger.cc"
"src/main.cpp"
"src/radio_433_eq/radio_433_eq.cpp"
"src/radio_433_eq/radio_button.cpp"
"src/satel_integra/satel_integra.cpp"
"src/satel_integra/satel_integra_handler.cpp"
"src/serialpi/serialpi.cpp"
"src/tasker/tasker.cpp"
"src/thermometer_container/thermometer_container.cpp"
"src/thread_functions/idom_thread.cpp"
"src/thread_functions/rs232_thread.h"
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
message("dane :")
message(${GIT_COMMIT_HASH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17  -DNOSSL -O3 ")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
file(GLOB SOURCE_FILES2 "src/command/*.cpp"
    "src/command/commandClass/*.cpp"
    "src/MENU/*.cpp")
add_executable(${PROJECT_NAME} ${LIBS_SRC} ${SOURCE_FILES} ${SOURCE_FILES2})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  wiringPiDev wiringPi rt pthread curl mpd mosquitto )
