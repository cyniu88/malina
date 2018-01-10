#include <RCSwitch.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


//PINs
#define ONE_WIRE_BUS 2
#define PIN 11
#define Radio_433_Tx 10
#define Radio_433_Rx 3
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
RCSwitch mySwitch = RCSwitch();


#include <OneWire.h>
#include <DallasTemperature.h>




// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
#define TEMPERATURE_PRECISION 12
// Ilość czujników
#define SENSORS_NUM 2
// Adresy czujników
// arrays to hold device addresses
DeviceAddress insideThermometer = { 
  0x28, 0x80, 0x3E, 0x64, 0x4, 0x0, 0x0, 0xD2 };
DeviceAddress outsideThermometer   = { 
  // 0x28, 0x8F, 0x9F, 0x63, 0x4, 0x0, 0x0, 0x8E };
  0x28, 0xFF, 0x07, 0x04, 0x81, 0x16, 0x03, 0x7A};
struct TEMPERATURE{
  double actual = 0;
  double old    = 0;
  int errorCounter = 0;
  bool sendEventFlag = false;
  double badValue = 0;
};

String command  = "z";
String value    = "0";
long int valueINT    = 0;

TEMPERATURE insideTemp;
TEMPERATURE outsideTemp;


bool LED_state = false;
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  sensors.begin();
  sensors.setResolution(insideThermometer , TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
  
  // Transmitter is connected to Arduino Pin #10 = Radio_433_TX 
mySwitch.enableTransmit(Radio_433_Tx);

}

void loop() {

  if (Serial.available() == 0){
    if (insideTemp.sendEventFlag == true ){
      insideTemp.sendEventFlag = false;
      Serial.print("RS232 error Temperature error inside: ");
      Serial.print(insideTemp.badValue);
      Serial.print(";");
      delay(1000);
    }
    if (outsideTemp.sendEventFlag == true){
      outsideTemp.sendEventFlag = false;
      Serial.print("RS232 error Temperature error outside: ");
      Serial.print(outsideTemp.badValue);
      Serial.print(";");
      delay(1000);
    }
  }
  if (Serial.available () > 4 )
  {
    command = Serial.readStringUntil(':');
    value = Serial.readStringUntil(';');
  }

  if (command == "LED")
  {
    int from,to,r, g, b;

    if (sscanf( value.c_str() , "[%d-%d-%d-%d-%d]", &from, &to,&r,&g,&b) == 5) {
      // do something with r, g, b
      LED_state = true;
      Serial.print("LEDY START ");
      Serial.print(';');
      delay(100);
      colorWipe(strip.Color(r, g, b), 5,from ,to);
    }
    else {
      Serial.print("LED START - ERROR");
      Serial.print(';');
    }
    command="z";
    valueINT=0;
  }
  ////////////////////////////////////////////////////////////////////////
  if (command == "LED_STOP")
  {
    Serial.print("LED STOP");
    Serial.print(';');
    strip.clear();
    strip.show();
    delay(100);
    LED_state = false;
    command="z";
    valueINT=0;
  }
  ///////////////////////////////////////////////////////////////////////
  if (command == "LED_CLEAR")
  {
    Serial.print("LED CLEAR");
    Serial.print(';');
    if (LED_state == false){
      delay(1000);
      strip.clear();
      strip.show();
      delay(100);
    }
    command="z";
    valueINT=0;

  }
  ////////////////////////////////////////////////////////////////////
  if (command == "getRS232")
  {
    Serial.print("ok;");
    delay(1000);
    Serial.print("RS232 error test msg comand;");
    outsideTemp.sendEventFlag = true;
    command="z";
    valueINT=0;
  }
  ////////////////////////////////////////////////////////////////////
  if (command=="temperature")
  {
    valueINT = value.toInt();
    sensors.requestTemperatures(); // Send the command to get temperatures

      insideTemp.actual  = sensors.getTempC(insideThermometer);
    outsideTemp.actual = sensors.getTempC(outsideThermometer);
    if ( insideTemp.actual< -60 ||  insideTemp.actual > 100)
    {
      insideTemp.badValue = insideTemp.actual;
      insideTemp.actual = insideTemp.old+0.005;
      ++insideTemp.errorCounter;
      insideTemp.sendEventFlag = true;
    }
    if ( outsideTemp.actual < -60 ||  outsideTemp.actual > 100)
    {
      outsideTemp.badValue = outsideTemp.actual;
      outsideTemp.actual = outsideTemp.old;
      ++outsideTemp.errorCounter;
      outsideTemp.sendEventFlag = true;
    }
    outsideTemp.old =  outsideTemp.actual;
    insideTemp.old  = insideTemp.actual;
    Serial.print(insideTemp.actual);
    Serial.print(':');
    Serial.print(outsideTemp.actual);
    Serial.print(';');
    command="z";
    valueINT=0;
  }
  /////////////////////////////////////////////////////////////
  if (command=="temperature_error")
  {
    Serial.print("Temperature error counter: OUT - ");
    Serial.print(String(outsideTemp.errorCounter));
    Serial.print(" | IN - ");
    Serial.print(String(insideTemp.errorCounter));
    Serial.print(';');
    command="z";
    valueINT=0;
  }
  /////////////////////////////////////////////////////////////
  if (command=="433MHz")
  {
    valueINT = value.toInt();
    mySwitch.send(valueINT, 24);
    Serial.print("send:");
    Serial.print(String(valueINT));
    Serial.print(";");
    command="z";
    valueINT=0;
  }
  //////////////////////////////////////////////////////////////
  if (command=="test")
  {
    valueINT = value.toInt();
    ++valueINT;
    ++valueINT;
    Serial.print("return:");
    //sprintf(c,valueINT);
    Serial.print(String(valueINT));
    Serial.print(';');
    command="z";
    valueINT=0;

  }
  //////////////////////////////////////////////////////////////
  if (command == "reset")
  {
    outsideTemp.errorCounter = 0;
    insideTemp.errorCounter = 0;
    Serial.print("reset done:00;");  
    command="z";
    valueINT=0;
  } 
  //////////////////////////////////////////////////////////////
  if (command=="clean")
  {
    // valueINT = value.toInt();
    // ++valueINT;
    //++valueINT;
    //Serial.print("return:");
    //sprintf(c,valueINT);
    // Serial.print(String(valueINT));
    //Serial.print(';');
    command="z";
    valueINT=0;
  }
  //////////////////////////////////////////////////////////////
  if (command !="z")
  {
    Serial.print ("unknown RS232 command: ");
    Serial.println (command);
    Serial.print(';');
    //delay(2000);
  }
  command="z";
}
/////////////////// end loop  //////////////////////////////////

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait,int from,int to) {

  if (to>strip.numPixels())
  {
    to = strip.numPixels();
  }
  for(uint16_t i=from; i<to; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
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
void rainbowCycle(uint8_t wait) {
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
void theaterChase(uint32_t c, uint8_t wait) {
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
void theaterChaseRainbow(uint8_t wait) {
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}




