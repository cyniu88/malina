TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    gnublin.cpp
INCLUDEPATH += -L/usr/local/include 
LIBS += -L/usr/local/lib    -lwiringPi -lmpd   -lpthread

HEADERS += \
    gnublin.h
