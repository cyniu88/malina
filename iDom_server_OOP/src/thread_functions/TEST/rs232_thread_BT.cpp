#include <gtest/gtest.h>
#include "../../iDom_server_OOP.h"
#include "../rs232_thread.h"
#include "test_data.h"
#include "../../../iDom_server_OOP/src/iDomTools/test/iDomTools_fixture.h"

std::mutex useful_F::mutex_buf;
std::mutex useful_F::mutex_who;

std::string buffer;

class rs232_thread_fixture : public iDomTOOLS_ClassTest
{

};

void SerialPi_set_recv_msg(const std::string& m){
    TEST_DATA::serial_b = m;
}

SerialPi::SerialPi(const std::string& a):m_serial_port(10){
    std::cout << "SerialPi() addres: " << a << std::endl;
}
SerialPi::~SerialPi(){
    std::cout << "~SerialPi()" << std::endl;
}

void SerialPi::begin(int serialSpeed){
    std::cout << "SerialPi::int() serialSpeed: " << serialSpeed << std::endl;
}

void SerialPi::print(const std::string& msg){
    std::cout << "SerialPi::print() msg: " << msg << std::endl;
    TEST_DATA::serial_sended = msg;
}
void SerialPi::flush(){
    std::cout << "SerialPi::flush()" << std::endl;
    TEST_DATA::serial_b.clear();
}

int SerialPi::available(){
    std::cout << "SerialPi::available() " << TEST_DATA::serial_b.size() << std::endl;
    return TEST_DATA::serial_b.size();
}

char SerialPi::read(){
    std::cout << "SerialPi::read()"<< std::endl;
    char r = TEST_DATA::serial_b.at(0);
    TEST_DATA::serial_b.erase(0,1);
    std::cout << "SerialPi::read() "<<r<< std::endl;
    return r;
}

bool useful_F::go_while = true;

TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_fixture_clock)
{
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = "9600";
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::CLOCK, iDomConst::FREE};
    EXPECT_EQ(test_who[1], iDomConst::FREE);
    test_data_rs232.pointer.ptr_who = test_who;

    SerialPi_set_recv_msg("OK");
    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"OK");
    Send_Recieve_rs232_thread(&test_data_rs232);

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_data_rs232.pointer.ptr_who[1], iDomConst::CLOCK);
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
}

TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_clock_empty_answer)
{
    useful_F::go_while = true;
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = "9600";
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::CLOCK, iDomConst::FREE};
    EXPECT_EQ(test_who[1], iDomConst::FREE);
    test_data_rs232.pointer.ptr_who = test_who;
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
    SerialPi_set_recv_msg("");
    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    Send_Recieve_rs232_thread(&test_data_rs232);

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_data_rs232.pointer.ptr_who[1], iDomConst::CLOCK);
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 1);
}

TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_RS232)
{
    useful_F::go_while = true;
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = "9600";
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::RS232, iDomConst::FREE};
    test_data_rs232.pointer.ptr_who = test_who;

    test_data_rs232.pointer.ptr_who[0] = iDomConst::RS232;
    EXPECT_EQ(test_who[0], iDomConst::RS232);
    SerialPi_set_recv_msg("OK;");
    Send_Recieve_rs232_thread(&test_data_rs232);

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_who[1], iDomConst::RS232);
}

TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_FREE)
{
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
    useful_F::go_while = true;
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = "9600";
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::FREE, iDomConst::FREE};
    test_data_rs232.pointer.ptr_who = test_who;

    test_data_rs232.pointer.ptr_who[0] = iDomConst::FREE;
    EXPECT_EQ(test_who[0], iDomConst::FREE);
    SerialPi_set_recv_msg("TEST;");
    Send_Recieve_rs232_thread(&test_data_rs232);

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_who[1], iDomConst::FREE);
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 1);
}
