#include <iostream>
#include <arduPi.h>


int main(int argc, char *argv[])
{
    SerialPi serial_ardu("/dev/serial0");
    serial_ardu.begin( 9600);
    serial_ardu.print("2234");

    if (serial_ardu.available())
    {
        std::cout << "jest cos"<<std::endl;
        while(serial_ardu.available())
        {
            std::cout <<serial_ardu.read();
        }
    }
    else
    {
        std::cout << "mie ma nic "<<std::endl;
    }

    std::cout << "Hello World!" << std::endl;
    return 0;
}
