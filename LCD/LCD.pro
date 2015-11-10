TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -L/usr/local/lib    -lrt -lpthread
SOURCES += main.cpp \
    ../arduPi/arduPi.cpp \
    ../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.cpp \
    ../iDom_server/src/LCD_c/lcd_c.cpp

HEADERS += \
    ../arduPi/arduPi.h \
    ../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.h \
    ../iDom_server/src/LCD_c/lcd_c.h

