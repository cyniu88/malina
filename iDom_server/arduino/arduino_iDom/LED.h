#include <Adafruit_NeoPixel.h>

class LED
{
  public:
  Adafruit_NeoPixel pixels ;
  
  LED(int count_LED, int led_pin);
  void init();
  void stop_led();
  void start(int R, int B, int G);
  private:
  int count_LED;
  
};
