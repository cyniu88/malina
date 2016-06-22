#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
class LED
{
  public:
  Adafruit_NeoPixel strip ;
  
  LED(int count_LED, int led_pin);
  void init();
  void stop_led();
  void start(unsigned int R, unsigned int B, unsigned int G,unsigned int from, unsigned int to);
  void colorWipe(uint32_t c, uint8_t wait);
  void rainbow(uint8_t wait);
  void rainbowCycle(uint8_t wait);
  void theaterChase(uint32_t c, uint8_t wait);
  void theaterChaseRainbow(uint8_t wait);
  uint32_t Wheel(byte WheelPos);
   uint32_t Color (uint8_t r, uint8_t g, uint8_t b);
   uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  unsigned int count_LED;
  
};
