TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -L/usr/local/lib    -lwiringPiDev -lwiringPi -lmpd   -llirc_client -pthread  -lrt -lpthread

SOURCES += src/iDom_server.cpp src/parser/parser.cpp src/serialib/serialib.cpp \
 src/c_connection/c_connection.cpp \
 src/functions/functions.cpp \
    src/c_connection/c_connection2.cpp \
    src/c_connection/c_connection_node.cpp \
    src/logger/logger.cc \
    src/c_master_irda/master_irda.cpp \
    src/functions/mpd_cli.cpp \
    src/blockQueue/blockqueue.cpp \
    src/c_master_irda/irda_logic.cpp \
    src/files_tree/files_tree.cpp \
    src/LCD_c/lcd_c.cpp \
    ../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.cpp \
    ../arduPi/arduPi.cpp


HEADERS += src/parser/parser.hpp src/serialib/serialib.h src/wiadomosc/wiadomosc.h \
 src/c_connection/c_connection.h \
 src/functions/functions.h \
    src/logger/logger.hpp \
    src/iDom_server.h \
    src/c_master_irda/master_irda.h \
    src/functions/mpd_cli.h \
    src/blockQueue/blockqueue.h \
    src/c_master_irda/irda_logic.h \
    src/files_tree/files_tree.h \
    src/LCD_c/lcd_c.h \
    ../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.h \
    ../arduPi/arduPi.h
