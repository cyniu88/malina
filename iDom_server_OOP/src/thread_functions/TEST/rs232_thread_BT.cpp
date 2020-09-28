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

TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_fixture_clock)
{
    std::array<Thread_array_struc,iDomConst::MAX_CONNECTION >test_ThreadArrayStruc;

    for (std::size_t i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
        test_ThreadArrayStruc.at(i).thread_socket = 0;
    test_my_data.main_THREAD_arr = &test_ThreadArrayStruc;

    useful_F::go_while = true;
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = 9600;
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int wh[2];
    test_data_rs232.pointer.ptr_who = wh;
    test_data_rs232.pointer.ptr_who[0] = iDomConst::CLOCK;
    test_data_rs232.pointer.ptr_who[1] = iDomConst::FREE;
    EXPECT_EQ(test_data_rs232.pointer.ptr_who[1], iDomConst::FREE);

    SerialPi_set_recv_msg("OK");
    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"OK");
    iDOM_THREAD::start_thread_RS232("RS232_THREAD",Send_Recieve_rs232_thread,&test_my_data,&test_data_rs232);

    sleep(1);
    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_data_rs232.pointer.ptr_who[1], iDomConst::CLOCK);
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
}

TEST_F(rs232_thread_fixture, send_Recieve_rs232_thread_clock_empty_answer)
{
    useful_F::go_while = true;
    thread_data_rs232 test_data_rs232;
    test_data_rs232.BaudRate = 9600;
    test_data_rs232.portRS232 = "test_port";
    test_data_rs232.portRS232_clock = "test_port_clock";
    unsigned int test_who[2] = {iDomConst::CLOCK, iDomConst::FREE};
    EXPECT_EQ(test_who[1], iDomConst::FREE);
    test_data_rs232.pointer.ptr_who = test_who;
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 0);
    SerialPi_set_recv_msg("");
    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    Send_Recieve_rs232_thread(&test_data_rs232, "RS232_thread");

    EXPECT_STREQ(TEST_DATA::serial_b.c_str(),"");
    EXPECT_EQ(test_data_rs232.pointer.ptr_who[1], iDomConst::CLOCK);
    EXPECT_EQ(useful_F::myStaticData->myEventHandler.run("RS232")->howManyEvent(), 1);
}

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
