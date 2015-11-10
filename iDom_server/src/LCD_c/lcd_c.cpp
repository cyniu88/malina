#include "lcd_c.h"

LCD_c::LCD_c()
{

}

void LCD_c::init (uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows){
    main_lcd.init(lcd_Addr,lcd_cols,lcd_rows);
}

void LCD_c::printstr(const char s[]){
    main_lcd.printstr(s);
}

void LCD_c::noBacklight(){
    main_lcd.noBacklight();
}
void LCD_c::clear()
{
    main_lcd.clear();
}
