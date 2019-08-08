#include "light_iDom.h"
#include "light_iDom_handler.h"

light_iDomHandler mainLightHandler;
//////////////////////////////////////////////////////////////////
//////////////////////       EDIT HERE      //////////////////////
///////////////    add a new lighting object here       //////////
//////////////////////////////////////////////////////////////////
//        light name, button pin,  relay pin            //////////
iDomLIGHT kuchnia("kuchnia", 6, 2);                     //////////
iDomLIGHT sypialnia("sypialnia", 7, 3);                 //////////
iDomLIGHT salon("salon", 8, 4);                         //////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
String command  = "z";
String value    = "0";
String ID = "0";

void setup() {
  Serial.begin(9600);
  Serial.print("iDom light v1;");
  Serial.print("iDom light - start adding light;");
  /////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////       EDIT HERE      ////////////////////////////////
  ///////////////////    insert new lighting object to handler     ////////////////////
  /////////////////////////////////////////////////////////////////////////////////////
  mainLightHandler.insert(&kuchnia);                                              /////
  mainLightHandler.insert(&sypialnia);                                            /////
  mainLightHandler.insert(&salon);                                                /////
  //support max 32 objects, if you want add more,                                 /////
  //please change constexpr int arrayMaxSize inside file light_iDom_handler.h     /////
  /////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////

  Serial.print("Done");
  delay(500);
  Serial.print("iDom light - setup pin;");
  mainLightHandler.setupPin();
  Serial.print("Done;");
  delay(500);
  Serial.print("iDom light - restore light;");
  mainLightHandler.restorePinState();
  Serial.print(";");
  Serial.print("Done;");
  Serial.print("iDom light setup done;");
}

void loop() {
  // put your main code here, to run repeatedly:
  mainLightHandler.run();


  if (Serial.available () > 4 )
  {
    command = Serial.readStringUntil(':');
    ID = Serial.readStringUntil(':');
    value = Serial.readStringUntil(';');
    Serial.flush();
    if (command == "light") {
      command = "";
      if (value == "ON") {
        mainLightHandler.m_lightArray[ID.toInt()]->lightON();
      }
      else if (value == "OFF") {
        mainLightHandler.m_lightArray[ID.toInt()]->lightOFF();
      }
    }
    else if (command == "show") {
      mainLightHandler.printAllNameAndState();
    }
    else {
      Serial.print ("unknown RS232 command: ");
      Serial.print (command);
      Serial.print(" ;");
    }
  }
}
