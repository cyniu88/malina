TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS +=   -L/usr/local/lib    -lwiringPiDev -lwiringPi -lmpd   -llirc_client -pthread  -lrt -lpthread -lboost_regex -lcurl

SOURCES += main.cpp \

