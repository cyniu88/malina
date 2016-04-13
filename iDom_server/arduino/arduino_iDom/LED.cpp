#include "LED.h"

LED::LED(int count_LED, int led_pin ){
this-> count_LED =  count_LED;
pixels = Adafruit_NeoPixel(this-> count_LED , led_pin, NEO_GRB + NEO_KHZ800);

}
void LED::init(){
  pixels.begin(); // Inicjalizacja biblioteki
}

void LED::stop_led()
{
  pixels.clear();
  pixels.show(); // Wysylamy dane do lancucha
}
void LED::start(int R = 255, int B = 255, int G =255 ){
 for(int i=0; i<count_LED; i++)
  {
    pixels.setPixelColor(i, R, B, G); // Dioda "i" oraz skladowe R=255 G=0 B=0
    pixels.show(); // Wysylamy dane do lancuchas
    delay(15); // Opoznienie 500ms przed zaswieceniem kolejnej diody
  } 
  
}
