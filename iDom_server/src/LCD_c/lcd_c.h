#ifndef LCD_C_H
#define LCD_C_H
#include "../arduPi/arduPi.h"
#include "../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.h"
#include <iostream>
#include <string>
class LCD_c
{
public:
    LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
    static int status;
    void init();
    void song_printstr();
    void noBacklight();
    void clear();
    void printString(int col, int row, std::string str);
    void printVolume (int vol);
    LiquidCrystal_I2C main_lcd ;
    void printSongName (std::string songName);
    void checkState();
private:
std::string intToStr(int n);
int lcd_state;
std::string row1;
std::string row2;
};

#endif // LCD_C_H
