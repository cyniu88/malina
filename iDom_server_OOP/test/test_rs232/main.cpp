#include <iostream>
#include <unistd.h>
#include "/home/pi/programowanie/iDom_server_OOP/src/SerialPi/serialpi.h"


int main(int argc, char *argv[])
{
    //SerialPi serial_ardu("/dev/ttyAMA0");
    SerialPi serial_ardu("/dev/ttyUSB0");
    serial_ardu.begin(9600);
    std::string s = "test:20;";
    //std::string s = "----";
    serial_ardu.print(s);
    sleep(1);
    if (serial_ardu.available())
    {
        std::cout << "jest cos"<<std::endl;
        while(serial_ardu.available())
        {
            std::cout << serial_ardu.read();
        }
    }
    else
    {
        std::cout << "mie ma nic "<<std::endl;
    }

    std::cout << "Hello World!" << std::endl;
    return 0;
}
