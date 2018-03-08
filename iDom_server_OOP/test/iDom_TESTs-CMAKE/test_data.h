#ifndef TEST_DATA_H
#define TEST_DATA_H

namespace TEST_DATA{
    static std::string return_send_to_arduino = "-2:-2";
    static std::string return_httpPost = "";
    static std::string return_httpPost_expect = "";
    static std::string return_viber_msg = "";
    static std::string serial_b = "NULL";
}

void SerialPi_set_recv_msg(std::string m);

#endif
