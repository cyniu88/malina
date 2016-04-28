#include <Adafruit_NeoPixel.h>


#include "LED.h" 
#define PIN 11
#define LICZBADIOD 60

//LEDY

LED _led(LICZBADIOD, PIN);

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

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
  0x28, 0x8F, 0x9F, 0x63, 0x4, 0x0, 0x0, 0x8E };


String command="z";

String value="0";
int valueINT=0;
int licznik =0;

void setup(void)
{

  Serial.begin(9600);
  _led.init();

  sensors.begin();
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);

}

void loop(void)
{ 
  if (Serial.available () > 6 )
  {
    command = Serial.readStringUntil(':');
    //Serial.println(command);
    value = Serial.readStringUntil(';');
     
  }

  if (command == "LED")
  {

    int from,to,r, g, b;

    if (sscanf( value.c_str() , "[%d.%d.%d.%d.%d]", &from, &to,&r,&g,&b) == 5) {

      // do something with r, g, b

      Serial.print("LEDY START ");  
      Serial.print(';');
      delay(100);
      _led.colorWipe(_led.Color(r, g, b), 50); // Red
    }
    else {

      Serial.print("LEDY START - ERROR");  
      Serial.print(';'); 
    } 
    command="z";
    valueINT=0;

  }
  if (command == "LED_STOP")
  {


    Serial.print("LEDY STOP");  
    Serial.print(';'); 
    delay(100);
    _led.stop_led();  
    command="z";
    valueINT=0;

  }
  if (command=="temperature")
  {
    valueINT = value.toInt();
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.print(sensors.getTempC(insideThermometer));
    Serial.print(':');  
    Serial.println(sensors.getTempC(outsideThermometer));
    Serial.print(';');  
    command="z";
    valueINT=0;

  }
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
  if (command !="z")
  {
    Serial.print ("unknown RS232 command: ");
    Serial.println (command);
    Serial.print(';');
    //delay(2000);
  }
  command="z";


  //++licznik;
  //Serial.println(licznik);
  //delay(1000);
}

