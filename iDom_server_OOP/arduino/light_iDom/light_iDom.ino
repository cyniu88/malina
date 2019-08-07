#include "light_iDom.h"
#include "light_iDom_handler.h"

light_iDomHandler mainLightHandler;

iDomLIGHT kuchnia("kuchnia", 6, 2);
iDomLIGHT sypialnia("sypialnia", 7, 3);
iDomLIGHT salon("salon", 8, 4);


void setup() {
  Serial.begin(9600);
  Serial.println("iDom light v1");  
  Serial.println("iDom light - start adding light");
  mainLightHandler.insert(&kuchnia);
  mainLightHandler.insert(&sypialnia);
  mainLightHandler.insert(&salon);
  Serial.println("Done");
  delay(500);
  Serial.println("iDom light - setup pin");
  mainLightHandler.setupPin();
  Serial.println("Done");
  delay(500);
  Serial.println("iDom light - restore light");
  mainLightHandler.restorePinState();
  Serial.println("Done");
  Serial.println("iDom light setup done");
}

void loop() {
  // put your main code here, to run repeatedly:
  mainLightHandler.run();

}
