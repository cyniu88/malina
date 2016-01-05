#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

String command="z";
 
String value="0";
int valueINT=0;
void setup(void)
{

  Serial.begin(9600);


  sensors.begin();
}

void loop(void)
{ 
  if (Serial.available () > 10 )
  {
    command = Serial.readStringUntil(':');
    //Serial.println(command);
    value = Serial.readStringUntil(';');
    //Serial.println(value);
    valueINT = value.toInt();
    //sensors.requestTemperatures(); // Send the command to get temperatures
    //Serial.print("Temperatura na Szuwarach: ");
    //Serial.println(sensors.getTempCByIndex(0));  
    //command="";
    //valueINT=0;
  }


  if (command=="temperature")
  {
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.print("Temperatura na Szuwarach: ");
    Serial.println(sensors.getTempCByIndex(0));
    Serial.print(';');  
    command="z";
    valueINT=0;
     
  }
  if (command !="z")
  {
   Serial.print ("unknown RS232 command ");
    Serial.println (command);
    Serial.print(';');
    //delay(2000);
  }
  command="z";
}

