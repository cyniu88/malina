#ifndef TEST_DATA_H
#define TEST_DATA_H

#include <iostream>

class TEST_DATA{
public:
    static std::string return_send_to_arduino;
    static std::string return_httpPost;
    static std::string return_httpPost_expect;
    static std::string return_viber_msg;
    static std::string serial_b;
    static std::string serial_sended;
    static std::string LCD_print;
    static int test_pin ;
};

void SerialPi_set_recv_msg(const std::string& m);
void SerialPi_set_serial_sended(const std::string& m);
void setReturnPinState(int i);

#endif
