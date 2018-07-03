#include <iostream>
#include <arduPi.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    SerialPi serial_ardu("/dev/ttyAMA0");
    serial_ardu.begin( 9600);
    serial_ardu.print("8888");
unistd::sleep(1);
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
