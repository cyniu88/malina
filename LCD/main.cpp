#include "../arduPi/arduPi.h"


#include "../liquidcrystal_i2c-for-ardupi/src/LiquidCrystal_I2C.h"
#include "../iDom_server/src/LCD_c/lcd_c.h"
//#include <wiringPi.h>
int main (){

    LCD_c m_lcd;

    m_lcd.init(0x27,16,2);
    delay(5000);

    m_lcd.printstr("kinia");
    delay (5000);
    m_lcd.noBacklight();
    return 0;
}

/*
void setup()
{

lcd.init(); // initialize the lcd

lcd.backlight();
//lcd.autoscroll();
//lcd.print(5,0,"CooCox");
lcd.print("dupa");
 lcd.setCursor(0,0);
delay(2000);
lcd.clear();
//lcd.showStr(5,0,"I2C LCD for");
//lcd.showStr(2, 1, "Raspberry Pi");
}

void loop() {
   lcd.noBacklight();
    printf("gasze \n");
    delay(1000);
   // lcd.backlight();
    printf("zapala,\n");
   //lcd.cursor();
//lcd.clear();

    lcd.print("testujemy  znaki  dlugie takie bardzo");
      //delay(1000);
}

int main() {

setup();
while(1) {

loop();
//
}
}
*/
