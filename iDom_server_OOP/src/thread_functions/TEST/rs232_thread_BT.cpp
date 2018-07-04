#include <gtest/gtest.h>
#include "../../iDom_server_OOP.h"
#include "../rs232_thread.h"
#include "/home/pi/programowanie/iDom_server_OOP/test/iDom_TESTs-CMAKE/test_data.h"


std::mutex useful_F::mutex_buf;
std::mutex useful_F::mutex_who;

std::string buffer;

void SerialPi_set_recv_msg(std::string m){
    TEST_DATA::serial_b = m;
}

SerialPi::SerialPi( std::string a){
    std::cout << "SerialPi() addres: "<< a << std::endl;
}
SerialPi::~SerialPi(){
    std::cout << "~SerialPi()"<< std::endl;
}

void SerialPi::begin(int serialSpeed){
    std::cout << "SerialPi::int() serialSpeed: "<< serialSpeed << std::endl;
}

void SerialPi::print(std::string msg){
    std::cout << "SerialPi::print() msg: "<< msg << std::endl;
    //TEST_DATA::serial_b = msg;
}
void SerialPi::flush(){
    std::cout << "SerialPi::flush()"<< std::endl;
    TEST_DATA::serial_b.clear();
}

int SerialPi::available(){
    std::cout << "SerialPi::available() "<<TEST_DATA::serial_b.size()<< std::endl;
    return TEST_DATA::serial_b.size();
}

char SerialPi::read(){
    std::cout << "SerialPi::read()"<< std::endl;
    char r = TEST_DATA::serial_b.at(0);
    TEST_DATA::serial_b.erase(0,1);
    std::cout << "SerialPi::read() "<<r<< std::endl;
    return r;
}

TASKER::TASKER(thread_data *my_data){
    std::cout << "TASKER::TASKER()"<< std::endl;
}

bool useful_F::go_while = true;

TEST(rs232_thread, send_Recieve_rs232_thread)
{
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = "9600";
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::CLOCK, iDomConst::FREE};
    test_data_rs232.pointer.ptr_who = test_who;

    TEST_DATA::serial_b = "OK";
    Send_Recieve_rs232_thread(&test_data_rs232);

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
}
