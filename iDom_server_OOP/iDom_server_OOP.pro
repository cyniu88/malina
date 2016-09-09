TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS += -O3   -std=c++14
LIBS +=   -L/usr/local/lib   -lwiringPiDev -lwiringPi   -lmpd   -llirc_client -pthread  -lrt -lpthread  -lcurl

SOURCES += src/iDom_server_OOP.cpp src/parser/parser.cpp \
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
    src/KEY/key.cpp


HEADERS += src/parser/parser.hpp  \
 src/c_connection/c_connection.h \
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
    src/KEY/key.h

