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

void LCD_c::noBacklight()
{
    std::cout << "LCD_c::noBacklight()"  <<std::endl;
}

void LCD_c::clear()
{
    std::cout << "LCD_c::clear()"  <<std::endl;
}

void LCD_c::printString(const std::string &row1, const std::string &row2, bool clear)
{
   std::cout << "Wyświetlam na LCD: " << std::endl << row1 << std::endl << row2 << std::endl;
}

