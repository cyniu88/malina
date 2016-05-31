
#include "SevSeg.h"

//Create an instance of the object.
SevSeg myDisplay;
int i = 0;
int dot_place = 0;
char command[4] ={'-','-','-','-'};
bool dot = false;

unsigned long  timer = millis();
//Create global variables


void setup()
{
  Serial.begin(4800);
  int displayType = COMMON_ANODE; //Your display is either common cathode or common anode


  //This pinout is for a bubble dispaly
  //Declare what pins are connected to the GND pins (cathodes)
  int digit1 = 7; //Pin 1
  int digit2 = 4; //Pin 10
  int digit3 = 3; //Pin 4
  int digit4 = A3; //Pin 6

  //Declare what pins are connected to the segments (anodes)
  int segA = 6; //Pin 12
  int segB = 2; //Pin 11
  int segC = A1; //Pin 3
  int segD = 13; //Pin 8
  int segE = 12; //Pin 2
  int segF = 5; //Pin 9
  int segG = A2; //Pin 7
  int segDP = A0; //Pin 5

  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);

  myDisplay.SetBrightness(50); //Set the display to 100% brightness level

   
}

void loop()
{
  if (Serial.available () >3 )
  {
    i=0;
    while (Serial.available ()){
      command[i]=(char)Serial.read();
      ++i;
    }
    if (command[0]=='S' && command[1]=='B'){
     
      myDisplay.SetBrightness( (((int)(command[2])-'0')*10)+((int)(command[3])-'0')     );
      
    }
     Serial.println("OK");
      

  }
 
  if (millis() >= timer ) {
      
    timer += 2000; 
    dot = !dot;
    if(dot){
    dot_place = 0;
     
    }
    else{
      dot_place = 2;
      
    }
  } 
  
  
   
  myDisplay.DisplayString(command, dot_place); //(numberToDisplay, decimal point location in binary number [4 means the third digit])

 
}

