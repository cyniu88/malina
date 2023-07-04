#include "../../liquidcrystal_i2c-for-ardupi/LiquidCrystal_I2C.h"
#include "lcd_c.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// Set the LCD address to 0x27 for a 16 chars and 2 line display

int main()
{
    
          std::this_thread::sleep_for(std::chrono::milliseconds(1000) );
    LiquidCrystal_I2C lcd(0x27, 16, 2);
    // initialize the LCD
    lcd.begin();

    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.setCursor(0,0);
    //lcd.printRight();
    lcd.printstr("Hello, world!");

    lcd.setCursor(0,1);
    for (int i = 0 ; i < 20; i++)
    {
          lcd.setCursor(0,1);
          lcd.printstr(std::to_string(i).c_str());  
          std::this_thread::sleep_for(std::chrono::milliseconds(1000) );
    }

    lcd.clear();
    lcd.printstr("iDom RELOAD");
int k;
std::cin >> k;
    lcd.noBacklight();
    return 0;
}