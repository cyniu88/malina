TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -L/usr/local/lib    -lwiringPi -lwiringPiDev -lmpd   -llirc_client -pthread
SOURCES += main.cpp

