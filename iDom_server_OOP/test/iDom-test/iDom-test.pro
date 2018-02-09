TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -O3   -std=c++14 -DNOSSL -DBT_TEST
LIBS +=   -L/usr/local/lib -lgtest -lgtest_main -lgmock -lwiringPiDev -lwiringPi -llirc_client   -lrt -lpthread  -lcurl  -lmpd

SOURCES += main.cpp \
    ../../libs/useful/usefull.cpp \
    ../../libs/useful/useful_bt.cpp \
    ../../libs/Statistic/statistic_test/statistic_BT.cpp \
    ../../src/RADIO_433_eq/test/radio_433_test.cpp \
    ../../libs/event_counter_test/event_counters/event_counters.cpp \
    ../../libs/event_counter_test/event_counters/event_counters_handler.cpp \
    ../../libs/event_counter_test/event_counters/event/event_mpd.cpp \
    ../../libs/event_counter_test/event_counters/event/event_pilot.cpp \
    ../../libs/event_counter_test/event_counters/event/event_command.cpp \
    ../../libs/event_counter_test/event_counters/event/event_unknown.cpp \
    ../../libs/event_counter_test/event_counters/event/new_connect_event.cpp \
    ../../src/RADIO_433_eq/radio_button.cpp \
    ../../src/RADIO_433_eq/radio_433_eq.cpp \
    ../../src/iDomTools/test/idomTools_BT.cpp \
    ../../src/iDomTools/idomtools.cpp \
    ../../libs/sunrise-sunset/sunriseset.cpp \
    ../../src/THERMOMETER_CONTAINER/thermometer_container.cpp \
    ../../src/iDomTools/idomtools_mpd.cpp \
    ../../libs/emoji/emoji.cpp \
    ../../src/blockQueue/blockqueue.cpp \
    ../../src/functions/functions2.cpp \
    ../../src/iDomStatus/idomstatus.cpp


HEADERS += \
    ../../src/logger/logger.hpp

