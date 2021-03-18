/*
EasySwitch_EasySensors_v1.014
2021.03.12 10:50:50
*/
#include "EasySwitch_sensors.h"
EasySwitch_sensors sensors;
void setup() { 
 sensors.begin();
}
void loop() {  
sensors.checking(); 
}
