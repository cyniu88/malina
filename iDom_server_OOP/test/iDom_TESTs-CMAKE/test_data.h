#ifndef TEST_DATA_H
#define TEST_DATA_H

namespace TEST_DATA{
    static std::string return_send_to_arduino = "-2:-2";
    static std::string return_httpPost = "ok.\n";
    static std::string return_httpPost_expect = "";
    static std::string return_viber_msg = "";
    static std::string serial_b = "NULL";
    static std::string serial_sended = "NULL";
    static int test_pin = 0;
}

void SerialPi_set_recv_msg(const std::string& m);

void setReturnPinState(int i);

#endif
