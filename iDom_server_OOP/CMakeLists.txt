cmake_minimum_required(VERSION 2.8)
LINK_DIRECTORIES(/usr/local/lib )

project(iDom_server-CMAKE)

set(INCLUDE_DIRECTORIES "json2/")
set(CMAKE_CXX_STANDARD 20)
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
    "liquidcrystal_i2c-for-ardupi/LiquidCrystal_I2C.cpp"
    "mqtt_mosquitto/MQTT_mosquitto/mqtt.cpp"
    "src/433MHz/rc_433mhz.cpp"
    "src/433MHz/RFLink/rflinkhandler.cpp"
    "src/blockQueue/blockqueue.cpp"
    "src/buderus/buderus.cpp"
    "src/c_connection/c_connection.cpp"
    "src/c_connection/c_connection2.cpp"
    "src/c_connection/c_connection_node.cpp"
    "src/command/command.cpp"
    "src/command/commandClass/command_433mhz.cpp"
    "src/command/commandClass/command_ardu.cpp"
    "src/command/commandClass/command_big.cpp"
    "src/command/commandClass/command_buderus.cpp"
    "src/command/commandClass/command_clock.cpp"
    "src/command/commandClass/command_cmd.cpp"
    "src/command/commandClass/command_event.cpp"
    "src/command/commandClass/command_gateway.cpp"
    "src/command/commandClass/command_hello.cpp"
    "src/command/commandClass/command_help.cpp"
    "src/command/commandClass/command_idom.cpp"
    "src/command/commandClass/command_ip.cpp"
    "src/command/commandClass/command_light.cpp"
    "src/command/commandClass/command_log.cpp"
    "src/command/commandClass/command_mpd.cpp"
    "src/command/commandClass/command_ok.cpp"
    "src/command/commandClass/command_program.cpp"
    "src/command/commandClass/command_put.cpp"
    "src/command/commandClass/command_shed.cpp"
    "src/command/commandClass/command_show.cpp"
    "src/command/commandClass/command_sleep.cpp"
    "src/command/commandClass/command_state.cpp"
    "src/command/commandClass/command_uptime.cpp"
    "src/command/commandClass/command_wifi.cpp"
    "src/command/commandClass/command_wifi.h"
    "src/command/commandClass/commandexit.cpp"
    "src/command/commandClass/commandrs232.cpp"
    "src/command/commandClass/commandtest.cpp"
    "src/command/commandhandler.cpp"
    "src/command/commandhandlergateway.cpp"
    "src/command/commandhandlermqtt.cpp"
    "src/command/commandhandlerroot.cpp"
    "src/command/commandhandlerrs232.cpp"
    "src/CRON/cron.cpp"
    "src/functions/functions.cpp"
    "src/functions/functions2.cpp"
    "src/functions/mpd_cli.cpp"
    "src/HOUSE/house_room_handler.cpp"
    "src/HOUSE/light_bulb.cpp"
    "src/HOUSE/room.cpp"
    "src/HTTP/http.cpp"
    "src/iDom_server_OOP.cpp"
    "src/iDomKEY_ACCESS/idomkey_access.cpp"
    "src/iDomSaveState/idom_save_state.cpp"
    "src/iDomStatus/idomstatus.cpp"
    "src/iDomTools/idomtools.cpp"
    "src/iDomTools/idomtools_mpd.cpp"
    "src/iDomTools/idomtools_system.cpp"
    "src/LCD_c/lcd_c.cpp"
    "src/logger/logger.cc"
    "src/main.cpp"
    "src/RADIO_433_eq/radio_433_eq.cpp"
    "src/RADIO_433_eq/radio_button.cpp"
    "src/SATEL_INTEGRA/satel_integra.cpp"
    "src/SATEL_INTEGRA/satel_integra_handler.cpp"
    "src/SerialPi/serialpi.cpp"
    "src/TASKER/tasker.cpp"
    "src/THERMOMETER_CONTAINER/thermometer_container.cpp"
    "src/thread_functions/iDom_thread.cpp"
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

ADD_LINK_OPTIONS(-rdynamic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a  -DNOSSL -O3 ")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
file(GLOB SOURCE_FILES2 "src/command/*.*"
    "src/command/commandClass/*.*"
    "src/MENU/*.*")
add_executable(${PROJECT_NAME} ${LIBS_SRC} ${SOURCE_FILES} ${SOURCE_FILES2})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  wiringPiDev wiringPi rt pthread curl mpd mosquitto i2c )
