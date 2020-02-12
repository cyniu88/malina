#include <iostream>
#include "../lcd_c.h"

LCD_c::LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows):
main_lcd(lcd_Addr,lcd_cols,lcd_rows){
    std::cout << "LCD_c::LCD_c()"  <<std::endl;
}

void LCD_c::set_print_song_state(int i){
    std::cout << "LCD_c::set_print_song_state(int i)" << i <<std::endl;
}

