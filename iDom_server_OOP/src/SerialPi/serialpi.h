#ifndef SERIALPI_H
#define SERIALPI_H
#include <wiringPi.h>
#include <wiringSerial.h>

#include <string>

#include  <exception>

class SerialPi{
    std::string m_address;
    int m_serial_port ;
public:
    SerialPi(std::string address);

    ~SerialPi();

    void begin(int budrate);
    int available();
    char read();
    void flush();
    void print(std::string m);
};

#endif // SERIALPI_H
