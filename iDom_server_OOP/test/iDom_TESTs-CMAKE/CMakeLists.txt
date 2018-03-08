cmake_minimum_required(VERSION 2.8)

LINK_DIRECTORIES(/usr/local/lib )
project(iDom_TESTs-CMAKE)

set(SOURCE_FILES  "main.cpp"
    "test_data.h"
    "../../libs/useful/usefull.cpp"
    "../../libs/useful/useful_bt.cpp"
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
    "../../src/iDomTools/idomtools.cpp"
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
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -DBT_TEST -DNOSSL")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  pthread gtest curl rt pthread gmock )
