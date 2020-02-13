#include <iostream>
#include "../lcd_c.h"
#include "../../liquidcrystal_i2c-for-ardupi/LiquidCrystal_I2C.h"
#include "../test/iDom_TESTs-CMAKE/test_data.h"

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows){
     std::cout << "LiquidCrystal_I2C::LiquidCrystal_I2C())"  <<std::endl;
}

LCD_c::LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows):
main_lcd(lcd_Addr,lcd_cols,lcd_rows)
{
    std::cout << "LCD_c::LCD_c()"  <<std::endl;
}

LCD_c::~LCD_c()
{
    std::cout << "LCD_c::~LCD_c()"  <<std::endl;
}

void LCD_c::set_print_song_state(int i){
    std::cout << "LCD_c::set_print_song_state(int i)" << i <<std::endl;
}

void LCD_c::set_lcd_STATE(int i){}

void LCD_c::checkState(){std::cout << "LCD_c::checkState()" << std::endl;}

void LCD_c::printString(bool clear, int col, int row, const std::string& str){
    std::cout << "LCD_c::printString() "<< str << std::endl;
    TEST_DATA::LCD_print = str;
}
