#include "LED.h"

LED::LED(int count_LED, int led_pin ){
this-> count_LED =  count_LED;
strip = Adafruit_NeoPixel(this-> count_LED , led_pin, NEO_GRB + NEO_KHZ800);

}
void LED::init(){
  strip.begin(); // Inicjalizacja biblioteki
  strip.show();  // Initialize all pixels to 'off
}

void LED::stop_led()
{
  strip.clear();
  strip.show(); // Wysylamy dane do lancucha
}
void LED::start(unsigned int R = 255,unsigned  int B = 255, unsigned int G =255,
unsigned int from=1, unsigned int to = 0 ){
  
  if ( to == 0)
  {
    to = count_LED;
  }
 for(unsigned int i=from; i<to; i++)
  {
    strip.setPixelColor(i, R, B, G); // Dioda "i" oraz skladowe R=255 G=0 B=0
    strip.show(); // Wysylamy dane do lancuchas
    delay(100); // Opoznienie 500ms przed zaswieceniem kolejnej diody
  } 
  
}

// Fill the dots one after the other with a color
void LED::colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void LED::rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void LED::rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void LED::theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void LED::theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

 uint32_t LED::Color (uint8_t r, uint8_t g, uint8_t b){
  return strip.Color (r,g,b);
}
   uint32_t LED::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
    return strip.Color (r,g,b);
  }
