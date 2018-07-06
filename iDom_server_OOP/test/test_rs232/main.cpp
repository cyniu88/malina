#include <iostream>
#include <map>
class A
{
public:
    A(){
        std::cout <<"konstruktor A"<< std::endl;
    }
    ~A(){
        std::cout <<"destruktor A"<< std::endl;
    }
};

int main()
{
    std::map < std::string , A*> k;
    k.insert(std::make_pair("fan", new A()));
    delete k["fan"];

    k.insert(std::make_pair("fan", new A()));

    delete k["fan"];
    return 0;
}
/*
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
*/
