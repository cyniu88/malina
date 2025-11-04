cmake_minimum_required(VERSION 2.8)
LINK_DIRECTORIES(/usr/local/lib )

project(iDom_TESTs-CMAKE)

# Enable testing
enable_testing()

set(INCLUDE_DIRECTORIES "../../json2/"
    "../../test/iDom_TESTs-CMAKE/"
    "../../test/libs_test/wiringPi")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${INCLUDE_DIRECTORIES})

set(LIBS_SRC
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
    "../../libs/circular_buffer/circular_buffer.h"
    "../../libs/circular_buffer/circular_buffer.cpp"
    "../../libs/circular_buffer/test/BT.cpp"
    "../../libs/TEST/testif.cpp"
     )

    set(SOURCE_FILES  "main.cpp" "test_data.h"
    "../../src/RADIO_433_eq/test/radio_433_test.cpp"
    "../../src/RADIO_433_eq/radio_button.cpp"
    "../../src/RADIO_433_eq/radio_433_eq.cpp"
    "../../src/iDomTools/test/idomTools_BT.cpp"
    "../../src/iDomTools/test/testJSON.h"
    "../../src/iDomTools/idomtools.cpp"
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
    "../../src/command/commandClass/command_light.cpp"
    "../../src/command/commandClass/TEST/command_light_BT.cpp"
    "../../src/command/commandClass/command_buderus.cpp"
    "../../src/c_connection/c_connection_node.cpp"
    "../../src/c_connection/c_connection2.cpp"
    "../../src/c_connection/c_connection.cpp"
    "../../src/LCD_c/test/lcd_c_stub.cpp"
    "../../src/logger/logger.cc"
    "../../mqtt_mosquitto/MQTT_mosquitto/TEST/mqtt_stub.cpp"
    "../../src/command/commandhandlermqtt.cpp"
    "../../src/command/commandhandlergateway.cpp"
    "../../src/iDomKEY_ACCESS/idomkey_access.cpp"
    "../../src/HOUSE/light_bulb.cpp"
    "../../src/HOUSE/house_room_handler.cpp"
    "../../src/HOUSE/TEST/BT.cpp"
    "../../src/buderus/buderus.cpp"
    "../../src/TASKER/tasker.cpp"
    "../../src/BIT_TEST/bit_main.cpp"
    "../../src/HOUSE/TEST/house_room_handler_stub.h"
    "../../src/HOUSE/room.cpp"
    "../../src/SATEL_INTEGRA/satel_integra.cpp"
    "../../src/SATEL_INTEGRA/BT/test.cpp"
    "../../src/SATEL_INTEGRA/satel_integra_handler.cpp"
    "../../src/SATEL_INTEGRA/BT/satel_integra_stub.cpp"
    "../../src/HTTP/test/http_BT.cpp"
    "../../src/HTTP/http.cpp"
    "../../src/dbClient/db-client.cpp"
    "../../src/dbClient/db-client-factory.cpp"
    "../../src/dbClient/test/bt.cpp" 
    "../../src/recuperator/recuperator.cpp"
    "../../src/CRON/cron.cpp"
    "../../src/CRON/BT/bt.cpp"
     )
file(GLOB SOURCE_FILES2 "../../src/command/*.*"
    "../../src/command/commandClass/TEST/*.*"
    "../../src/command/commandClass/*.*"
    "../../src/command/test/*.*"
    "../../src/MENU/*.*"
    "../../src/MENU/*/*")

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

add_link_options(-rdynamic)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -DBT_TEST -DNOSSL -fsanitize=address,leak,unreachable,undefined  -fsanitize-undefined-trap-on-error -fno-sanitize-recover=all -gdwarf-4 -g -O -fprofile-instr-generate -fcoverage-mapping")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SOURCE_FILES2} ${LIBS_SRC})

# Link libraries
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread gtest gmock curl rt pthread)

# Add tests
add_test(NAME iDom_TESTs-CMAKE COMMAND ${PROJECT_NAME})
