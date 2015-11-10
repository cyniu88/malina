#ifndef LCD_C_H
#define LCD_C_H
#include "../arduPi/arduPi.h"
#include "../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.h"
//
class LCD_c
{
public:
    LCD_c();
    static int status;
    void init(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
    void printstr(const char[]);
    void noBacklight();
    void clear();
private:
    LiquidCrystal_I2C main_lcd ;

};

#endif // LCD_C_H
