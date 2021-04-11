        
        /*    ___________________________________
         GND | o o |IrDa DATA                    |
         DG  | o o |STB                          |
             | o o |CLK                          |
             | o o |                             |
             | o o |5V                           |
             |_____|_____________________________|
         */
        
        #include <IRremote.h>
        #include "nbox_api.h"
        
        int IR_RECEIVE_PIN = 11;
        IRrecv IrReceiver(IR_RECEIVE_PIN);
        
        NBOX_API apiNbox;
        
        int vcc = 2;
        int stb = 3;
        int clk = 4;
        int dg = 5;
        
        #define ON 1
        #define OFF 0
        
        String command = "z";
        String value;
        unsigned long timeoutMillis = 0;
        int timeoutTime = 0;

                
        void setup() {
          Serial.begin(9600);
          apiNbox.setup(clk, stb, dg);
          IrReceiver.enableIRIn();  // Start the receiver
         
          apiNbox.segmentPrint("----", OFF, ON , OFF, OFF);
          apiNbox.setPowerLed(false);
          apiNbox.setAlarmLed(false);
          apiNbox.setClockLed(false);
          apiNbox.setAtLed(false);
          apiNbox.ledBrightness(ON, 100);
        }
        // the loop routine runs over and over again forever:
        void loop() {          
          apiNbox.notifyWhenButtonChange();
          
          if (IrReceiver.decode()) {
            IrReceiver.printResultShort(&Serial);
            Serial.println();
            IrReceiver.resume(); // Receive the next value
          }
          
          if (Serial.available () > 4 )
          {
            command = Serial.readStringUntil(':');
            value = Serial.readStringUntil(';');
          }
          if (command == "CLOCK")
          {
            apiNbox.printClock(value);
            command = "z";
            //value = 0;
          }
          if (command == "TEXT") {
            apiNbox.segmentPrint(value, 0, 0, 0, 0);
            command = "z";
          }
          if (command == "TEMPERATURE") {
            apiNbox.segmentPrint(value, 0, 0, 1, 0);
            command = "z";
          }
          if (command == "LED_POWER") {
            apiNbox.setPowerLed(value.toInt());
            command = "z";
          }
          if (command == "LED_CLOCK") {
            apiNbox.setClockLed(value.toInt());
            command = "z";
          }
          if (command == "LED_AT") {
            apiNbox.setAtLed(value.toInt());
            command = "z";
          }
          if (command == "LED_ALARM") {
            apiNbox.setAlarmLed(value.toInt());
            command = "z";
          }
          if (command == "LED_BRIG") {
            apiNbox.ledBrightness(ON, value.toInt());
            command = "z";
          }
          if (command == "TIMEOUT")
          {
            timeoutMillis = millis();
            timeoutTime = value.toInt();
            command = "z";
          }
          if (command != "z")
          {
            Serial.print ("unknown RS232 command: ");
            Serial.println (command);
            command = "z";
            //delay(2000);
          }
          if (timeoutTime != 0 && (millis() - timeoutMillis) > timeoutTime) 
          {
            Serial.print("KEY_PAD:");
            Serial.println("778");
            timeoutTime = 0;
          }
        
        } //loop

