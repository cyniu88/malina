#include <gtest/gtest.h>
#include "../../iDom_server_OOP.h"
#include "../rs232_thread.h"

std::mutex C_connection::mutex_buf;
std::mutex C_connection::mutex_who;

std::string buffer;
namespace TEST_DATA{

static std::string serial_b = "TEST";
}

SerialPi::SerialPi( char* a){
    std::cout << "SerialPi() addres: "<< a << std::endl;
}

void SerialPi::begin(int serialSpeed){
    std::cout << "SerialPi::int() serialSpeed: "<< serialSpeed << std::endl;
}

void SerialPi::print(char const* msg){
    std::cout << "SerialPi::print() msg: "<< msg << std::endl;
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
    return r;
}

TASKER::TASKER(thread_data *my_data){
    std::cout << "TASKER::TASKER()"<< std::endl;
}
void TASKER::dataFromRS232(std::string bufor)
{
    std::cout << "TASKER::dataFromRS232()"<< bufor<< std::endl;
}

bool useful_F::go_while = true;

TEST(rs232_thread, send_Recieve_rs232_thread)
{
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = "9600";
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::FREE, iDomConst::FREE};
    test_data_rs232.pointer.ptr_who = test_who;

    TEST_DATA::serial_b = "Test;";
    Send_Recieve_rs232_thread(&test_data_rs232);

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
}
