#include "serialpi.h"
#include <stdio.h>

SerialPi::SerialPi(const std::string &address) : m_address(address), m_serial_port(0)
{
}

SerialPi::~SerialPi()
{
    serialClose(m_serial_port);
}

void SerialPi::begin(int budrate)
{
    if ((m_serial_port = serialOpen(m_address.c_str(), budrate)) < 0) /* open serial port */
    {
        throw "cannot open serial port"; // todo add exceptions
    }
}

int SerialPi::available()
{
    return serialDataAvail(m_serial_port);
}

char SerialPi::read()
{
    return static_cast<char>(serialGetchar(m_serial_port));
}

void SerialPi::flush()
{
    serialFlush(m_serial_port);
}
void SerialPi::print(const std::string &m)
{
    std::lock_guard<std::mutex> lock(mutex_rs232);
    serialPrintf(m_serial_port, m.c_str());
}
