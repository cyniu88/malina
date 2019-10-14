cmake_minimum_required(VERSION 2.8)
LINK_DIRECTORIES(/usr/local/lib )

project(iDom_TESTs-CMAKE)

set(INCLUDE_DIRECTORIES "../../json2/" #"../../json/single_include/nlohmann/"
    "../../test/iDom_TESTs-CMAKE/"
    "../../test/libs_test/wiringPi")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${INCLUDE_DIRECTORIES})

set(SOURCE_FILES  "main.cpp"
    "../../libs/useful/usefull.cpp"
    "../../libs/useful/test/useful_bt.cpp"
    "../../libs/Statistic/statistic_test/statistic_BT.cpp"
    "../../libs/event_counters/event_counters_handler.cpp"
    "../../libs/event_counters/event/event_pilot.cpp"
    "../../libs/event_counters/event_counters.cpp"
    "../../libs/event_counters/event/new_connect_event.cpp"
    "../../libs/event_counters/event/event_command.cpp"
    "../../libs/event_counters/event/event_unknown.cpp"
    "../../libs/event_counters/event/event_mpd.cpp"
    "../../libs/event_counters/test/event_BT.cpp"
    "../../libs/sunrise-sunset/sunriseset.cpp"
    "../../libs/emoji/emoji.cpp"
    "../../libs/generator/generator.cpp"
    "../../libs/generator/TEST/generator_BT.cpp"
    "../../libs/alarm/alarm.cpp"
    "../../libs/alarm/TEST/alarm_BT.cpp"
    "../../src/RADIO_433_eq/test/radio_433_test.cpp"
    "../../src/RADIO_433_eq/radio_button.cpp"
    "../../src/RADIO_433_eq/radio_433_eq.cpp"
    "../../src/iDomTools/test/idomTools_BT.cpp"
    "../../src/iDomTools/test/lightning_BT.cpp"
    "../../src/iDomTools/test/iDom_server_BT.cpp"
    "../../src/iDomTools/test/testJSON.h"
    "../../src/iDomTools/idomtools.cpp"
    "../../src/iDomTools/lightning.cpp"
    "../../src/iDomSaveState/idom_save_state.cpp"
    "../../src/iDomSaveState/test/iDomSaveState_BT.cpp"
    "../../src/THERMOMETER_CONTAINER/thermometer_container.cpp"
    "../../src/THERMOMETER_CONTAINER/test/thermometer_container_BT.cpp"
    "../../src/iDomTools/idomtools_mpd.cpp"
    "../../src/iDomTools/idomtools_system.cpp"
    "../../src/blockQueue/blockqueue.cpp"
    "../../src/blockQueue/test/blockqueue_BT.cpp"
    "../../src/functions/functions2.cpp"
    "../../src/functions/tests/functions_stub.cpp"
    "../../src/functions/tests/functions_BT.cpp"
    "../../src/iDomStatus/idomstatus.cpp"
    "../../src/iDomTools/test/idomTools_BT.cpp"
    "../../src/iDomTools/test/iDomTools_fixture.h"
    "../../src/thread_functions/rs232_thread.h"
    "../../src/thread_functions/TEST/rs232_thread_BT.cpp"
    "../../src/thread_functions/iDom_thread.h"
    "../../src/thread_functions/iDom_thread.cpp"
    "../../src/433MHz/RFLink/TEST/rflinkhandler_BT.cpp"
    "../../src/433MHz/RFLink/rflinkhandler.cpp"
    "../../src/command/command.cpp"
    "../../src/command/commandClass/command_ardu.cpp"
    "../../src/command/commandClass/command_433mhz.cpp"
    "../../src/command/commandClass/command_idom.cpp"
    "../../src/c_connection/test/c_connection_BT.cpp"
    "../../src/command/commandClass/command_wifi.cpp"
    "../../src/command/commandClass/command_gateway.cpp"
    "../../src/command/commandClass/TEST/command_gateway_bt.cpp"
    "../../src/command/commandClass/TEST/command_wifi_bt.cpp"
    "../../src/c_connection/c_connection_node.cpp"
    "../../src/c_connection/c_connection2.cpp"
    "../../src/c_connection/c_connection.cpp"
    "../../src/LCD_c/test/lcd_c_stub.cpp"
    "../../src/c_irda_logic/c_irda_logic.h"
    "../../src/c_irda_logic/c_irda_logic.cpp"
    "../../src/c_irda_logic/test/c_irda_logic_BT.cpp"
    "../../src/files_tree/files_tree.cpp"
    "../../src/menu_tree/menu_tree.cpp"
    "../../src/logger/logger.cc"
    "../../src/KEY/key.cpp"
    "../../src/KEY/TEST/key_BT.cpp"
    "../../mqtt_mosquitto/MQTT_mosquitto/TEST/mqtt_stub.cpp"
    "../../src/command/commandhandlermqtt.cpp"
    "../../src/command/commandhandlergateway.cpp"
    "../../src/iDomKEY_ACCESS/idomkey_access.cpp"
    "../../src/home_lighting/light_bulb.cpp"
    "../../src/home_lighting/house_room.cpp"
    "../../src/home_lighting/house_lighting_handler.cpp"
    "../../src/home_lighting/TEST/BT.cpp"
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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DBT_TEST -DNOSSL -O3 -fprofile-instr-generate -fcoverage-mapping")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
file(GLOB SOURCE_FILES2 "../../src/command/*.cpp"
    "../../src/command/commandClass/TEST/*.cpp"
    "../../src/command/commandClass/*.cpp"
    "../../src/command/test/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SOURCE_FILES2})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  pthread gtest curl rt pthread )
