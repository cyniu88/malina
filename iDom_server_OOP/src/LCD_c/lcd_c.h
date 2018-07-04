#ifndef LCD_C_H
#define LCD_C_H

#include <iostream>
#include <string>
#include <regex>
#include "/home/pi/programowanie/liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.h"

class LCD_c
{
public:
    LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
    static int status;
    void init();
    void set_lcd_STATE (int i);
    void set_print_song_state (int i);
    void song_printstr();
    void printRadioName(bool clear, int col, int row, std::string st);
    void noBacklight();
    void clear();
    void printString(bool clear, int col, int row, std::string str);
    void printVolume (int vol);

    LiquidCrystal_I2C main_lcd ;
    void printSongName (std::string songName);
    void checkState();
    bool play_Y_N = false;
private:
    //std::string intToStr(int n);
    int lcd_state = 0;
    int print_song_state = 0;
    int rePrint = 0;
    std::string row1;
    std::string row2;
    std::string radioName;

};

#endif // LCD_C_H
