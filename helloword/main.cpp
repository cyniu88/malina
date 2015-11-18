#include <iostream>
#include <dirent.h>
#include <cstdio>
#include <cerrno>
#include <string>
#include <set>
#include <iomanip>
//#include <../arduPi/arduPi.h>
#include <iostream>
#include <wiringPi.h>
using namespace std;
int incomingByte = 0;   // for incoming serial data

void setup() {
      //  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        if (wiringPiSetup () == -1)
            return  ;

        //WiringPi initalize
        //if (wiringPiSetupSys () == -1)
        //   return ;
        pinMode(2, OUTPUT);
}

void loop() {

//        // send data only when you receive data:
//        if (Serial.available() > 0) {
//                // read the incoming byte:AVR::
//            for (;Serial.available() > 0;)
//            {
//            incomingByte = Serial.read();
//                std::cout << "odebralem "<< incomingByte<<std::endl;
//             }
//                // say what you got:
//                //Serial.print("I received: ");
//                //Serial.println(incomingByte, DEC);
//        }
//        Serial.write('a');
//         //delay(3000);
//        sleep(10);
}

main( int argc, char ** argv ) {
    setup();
    std::cout << "koniec setup"<<std::endl;
//    while(1){
//        loop();
//    }
    return (0);
}
