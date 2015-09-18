TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -L/usr/local/lib    -lwiringPi -lmpd

SOURCES += main.cpp \
    c_mpd_cli.cpp

HEADERS += \
    c_mpd_cli.h

