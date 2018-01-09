TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS += -O3   -std=c++14 -DNOSSL
LIBS +=   -L/usr/local/lib   -lwiringPiDev -lwiringPi -llirc_client   -lrt -lpthread  -lcurl  -lmpd

SOURCES += src/iDom_server_OOP.cpp\
     src/c_connection/c_connection.cpp \
    src/functions/functions.cpp \
    src/c_connection/c_connection2.cpp \
    src/c_connection/c_connection_node.cpp \
    src/logger/logger.cc \
    src/c_master_irda/master_irda.cpp \
    src/functions/mpd_cli.cpp \
    src/blockQueue/blockqueue.cpp \
    src/files_tree/files_tree.cpp \
    src/LCD_c/lcd_c.cpp \
    ../arduPi/arduPi.cpp \
    ../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.cpp \
    src/c_irda_logic/c_irda_logic.cpp \
    src/menu_tree/menu_tree.cpp \
    src/CRON/cron.cpp \
    src/KEY/key.cpp \
    src/command/command.cpp \
    src/command/commandClass/commandexit.cpp \
    src/command/commandClass/commandtest.cpp \
    src/command/commandhandler.cpp \
    src/command/commandClass/command_mpd.cpp \
    src/command/commandClass/commandrs232.cpp \
    src/command/commandClass/command_uptime.cpp \
    src/command/commandClass/command_big.cpp \
    src/command/commandClass/command_clock.cpp \
    src/command/commandClass/command_cmd.cpp \
    src/command/commandClass/command_hello.cpp \
    src/command/commandClass/command_help.cpp \
    src/command/commandClass/command_ip.cpp \
    src/command/commandClass/command_ok.cpp \
    src/command/commandClass/command_put.cpp \
    src/command/commandClass/command_show.cpp \
    src/command/commandClass/command_sleep.cpp \
    libs/event_counter_test/event_counters/event/event_unknown.cpp \
    libs/event_counter_test/event_counters/event/new_connect_event.cpp \
    libs/event_counter_test/event_counters/event_counters_handler.cpp \
    libs/event_counter_test/event_counters/event_counters.cpp \
    src/command/commandClass/command_event.cpp \
    libs/event_counter_test/event_counters/event/event_mpd.cpp \
    libs/event_counter_test/event_counters/event/event_pilot.cpp \
    libs/event_counter_test/event_counters/event/event_command.cpp \
    src/iDomTools/idomtools.cpp \
    src/command/commandClass/command_idom.cpp \
    libs/sunrise-sunset/sunriseset.cpp \
    src/command/commandClass/command_stop.cpp \
    src/iDomTools/idomtools_mpd.cpp \
    src/command/commandhandlerroot.cpp \
    src/command/commandClass/command_log.cpp \
    src/TASKER/tasker.cpp \
    src/command/commandhandlerrs232.cpp \
    src/iDomStatus/idomstatus.cpp \
    libs/useful/usefull.cpp \
    src/command/commandClass/command_state.cpp \
    libs/viberAPI/viber_api.cpp \
    libs/facebookAPI/facebookAPI.cpp \
    src/THERMOMETER_CONTAINER/thermometer_container.cpp \
    libs/config_parser/parser.cpp \
    ftplibpp/ftplib.cpp \
    libs/emoji/emoji.cpp


HEADERS += src/c_connection/c_connection.h \
    src/functions/functions.h \
    src/logger/logger.hpp \
    src/iDom_server_OOP.h \
    src/c_master_irda/master_irda.h \
    src/functions/mpd_cli.h \
    src/blockQueue/blockqueue.h \
    src/files_tree/files_tree.h \
    src/LCD_c/lcd_c.h \
    ../arduPi/arduPi.h \
    ../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.h \
    src/c_irda_logic/c_irda_logic.h \
    src/wiadomosc/wiadomosc.h \
    src/menu_tree/menu_tree.h \
    src/CRON/cron.hpp \
    src/KEY/key.h \
    src/command/commandhandler.h \
    src/command/command.h \
    src/command/command_unit/commandtest.h \
    src/command/command.h \
    src/command/commandClass/commandexit.h \
    src/command/commandClass/commandtest.h \
    src/command/commandClass/command_mpd.h \
    src/command/commandClass/commandrs232.h \
    src/command/commandClass/command_uptime.h \
    src/command/commandClass/command_big.h \
    src/command/commandClass/command_clock.h \
    src/command/commandClass/command_cmd.h \
    src/command/commandClass/command_hello.h \
    src/command/commandClass/command_help.h \
    src/command/commandClass/command_ip.h \
    src/command/commandClass/command_ok.h \
    src/command/commandClass/command_put.h \
    src/command/commandClass/command_show.h \
    src/command/commandClass/command_sleep.h \
    libs/event_counter_test/event_counters/event/event_unknown.h \
    libs/event_counter_test/event_counters/event/new_connect_event.h \
    libs/event_counter_test/event_counters/event_counters_handler.h \
    libs/event_counter_test/event_counters/event_counters.h \
    src/command/commandClass/command_event.h \
    libs/event_counter_test/event_counters/event/event_mpd.h \
    libs/event_counter_test/event_counters/event/event_pilot.h \
    libs/event_counter_test/event_counters/event/event_command.h \
    src/iDomTools/idomtools.h \
    src/command/commandClass/command_idom.h \
    libs/sunrise-sunset/sunriseset.h \
    src/command/commandClass/command_stop.h \
    src/command/commandhandlerroot.h \
    src/command/commandClass/command_log.h \
    src/TASKER/tasker.h \
    src/command/commandhandlerrs232.h \
    libs/useful/useful.h \
    src/iDomStatus/idomstatus.h \
    src/command/commandClass/command_state.h \
    libs/viberAPI/viber_api.h \
    libs/facebookAPI/facebookAPI.h \
    libs/Statistic/statistic.h \
    src/THERMOMETER_CONTAINER/thermometer_container.h \
    libs/config_parser/parser.hpp \
    ftplibpp/ftplib.h \
    libs/emoji/emoji.h

DISTFILES +=

