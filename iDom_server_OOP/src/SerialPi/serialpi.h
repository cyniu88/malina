#ifndef SERIALPI_H
#define SERIALPI_H
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string>
#include <exception>
#include <mutex>

class SerialPi{
    std::string m_address;
    int m_serial_port;
    std::mutex mutex_rs232;
public:
    explicit SerialPi(const std::string &address);
    ~SerialPi();

    void begin(int budrate);
    int available();
    char read();
    void flush();
    void print(const std::string& m);
};

#endif // SERIALPI_H
