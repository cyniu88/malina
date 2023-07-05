#include <iostream>
#include "../lcd_c.h"

LCD_c::LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) 
{
    std::cout << "LCD_c::LCD_c()" << std::endl;
}

LCD_c::~LCD_c()
{
    std::cout << "LCD_c::~LCD_c()" << std::endl;
}

void LCD_c::noBacklight()
{
    std::cout << "LCD_c::noBacklight()" << std::endl;
}

void LCD_c::clear()
{
    std::cout << "LCD_c::clear()" << std::endl;
}

void LCD_c::scrollLeft()
{
    std::cout << "LCD_c::scrollLeft()" << std::endl;
}

void LCD_c::scrollRight()
{
    std::cout << "LCD_c::scrollRight()" << std::endl;
}

void LCD_c::printString(const std::string &row1, const std::string &row2, bool clear)
{
    std::cout << "Wyświetlam na LCD: " << std::endl
              << row1 << std::endl
              << row2 << std::endl;
}

std::string LCD_c::getData() const
{
    return " data test";
}

std::string LCD_c::dump() const
{
    return "LCD STUB DUMP";
}