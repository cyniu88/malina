#ifndef LCD_C_H
#define LCD_C_H

#include <iostream>
#include <string>
#include <regex>
#include "../liquidcrystal_i2c-for-ardupi/LiquidCrystal_I2C.h"
#include "../idom_api.h"

class LCD_c : public iDom_API
{
public:
    LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
    ~LCD_c();
    void init();
    void noBacklight();
    void clear();
    void printString(const std::string& row1, const std::string& row2, bool clear = false);
    std::string getData();
    std::string dump() const override;
private:
    std::string m_printed = "NULL";
    LiquidCrystal_I2C main_lcd;
};

#endif // LCD_C_H
