TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -L/usr/local/lib    -lwiringPi -lmpd   -llirc_client -pthread

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
    src/functions/master_LCD.cpp


HEADERS += src/parser/parser.hpp src/serialib/serialib.h src/wiadomosc/wiadomosc.h \
 src/c_connection/c_connection.h \
 src/functions/functions.h \
    src/logger/logger.hpp \
    src/iDom_server.h \
    src/c_master_irda/master_irda.h \
    src/functions/mpd_cli.h \
    src/blockQueue/blockqueue.h \
    src/c_master_irda/irda_logic.h \
    src/functions/master_LCD.h
