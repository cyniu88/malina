TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp
INCLUDEPATH += -L/usr/local/include 
LIBS += -L/usr/local/lib    -lwiringPi -lmpd
