TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lcurl
QMAKE_CXXFLAGS += -O3   -std=c++14 -DNOSSL
SOURCES += main.cpp \
    ../iDom_server_OOP/ftplibpp/ftplib.cpp

HEADERS += \
    ../iDom_server_OOP/ftplibpp/ftplib.h
