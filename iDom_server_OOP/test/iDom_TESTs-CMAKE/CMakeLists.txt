cmake_minimum_required(VERSION 2.8)

LINK_DIRECTORIES(/usr/local/lib )
project(iDom_TESTs-CMAKE)
include_directories("../../libs/json/single_include/nlohmann/")
set(SOURCE_FILES  "main.cpp"
    "../../libs/useful/usefull.cpp"
    "../../libs/useful/test/useful_bt.cpp"
    "../../libs/Statistic/statistic_test/statistic_BT.cpp"
    "../../src/RADIO_433_eq/test/radio_433_test.cpp"
    "../../libs/event_counter_test/event_counters/event_counters_handler.cpp"
    "../../libs/event_counter_test/event_counters/event/event_pilot.cpp"
    "../../libs/event_counter_test/event_counters/event_counters.cpp"
    "../../libs/event_counter_test/event_counters/event/new_connect_event.cpp"
    "../../libs/event_counter_test/event_counters/event/event_command.cpp"
    "../../libs/event_counter_test/event_counters/event/event_unknown.cpp"
    "../../libs/event_counter_test/event_counters/event/event_mpd.cpp"
    "../../src/RADIO_433_eq/radio_button.cpp"
    "../../src/RADIO_433_eq/radio_433_eq.cpp"
    "../../src/iDomTools/test/idomTools_BT.cpp"
    "../../src/iDomTools/test/lightning_BT.cpp"
    "../../src/iDomTools/test/testJSON.h"
    "../../src/iDomTools/idomtools.cpp"
    "../../src/iDomTools/lightning.cpp"
    "../../libs/sunrise-sunset/sunriseset.cpp"
    "../../src/THERMOMETER_CONTAINER/thermometer_container.cpp"
    "../../src/iDomTools/idomtools_mpd.cpp"
    "../../libs/emoji/emoji.cpp"
    "../../src/blockQueue/blockqueue.cpp"
    "../../src/functions/functions2.cpp"
    "../../src/functions/tests/functions_BT.cpp"
    "../../src/iDomStatus/idomstatus.cpp"
    "../../src/iDomTools/test/idomTools_BT.cpp"
    "../../src/thread_functions/rs232_thread.h"
    "../../src/thread_functions/TEST/rs232_thread_BT.cpp"
    "../../src/433MHz/RFLink/TEST/rflinkhandler_BT.cpp"
    "../../src/433MHz/RFLink/rflinkhandler.cpp"
    "../../src/command/command.cpp"
    "../../src/command/commandClass/command_ardu.cpp"
    "../../src/command/commandClass/TEST/command_ardu_BT.cpp"
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DBT_TEST -DNOSSL")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  pthread gtest curl rt pthread gmock)
