#include "../../iDom_server_OOP.h"
#include "../rs232_thread.h"
#include "test_data.h"
#include <gtest/gtest.h>

class rs232_thread_fixture
{};

/*
TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_RS232)
{
    useful_F::go_while = true;
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = 9600;
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::RS232, iDomConst::FREE};
    test_data_rs232.pointer.ptr_who = test_who;

    test_data_rs232.pointer.ptr_who[0] = iDomConst::RS232;
    EXPECT_EQ(test_who[0], iDomConst::RS232);
    SerialPi_set_recv_msg("OK;");
    Send_Recieve_rs232_thread(&test_data_rs232,"RS232_thread");

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_who[1], iDomConst::RS232);
}

TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_FREE)
{
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
    useful_F::go_while = true;
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = 9600;
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::FREE, iDomConst::FREE};
    test_data_rs232.pointer.ptr_who = test_who;

    test_data_rs232.pointer.ptr_who[0] = iDomConst::FREE;
    EXPECT_EQ(test_who[0], iDomConst::FREE);
    SerialPi_set_recv_msg("TEST;");
    Send_Recieve_rs232_thread(&test_data_rs232,"RS232_thread");

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_who[1], iDomConst::FREE);
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 1);
}
*/
