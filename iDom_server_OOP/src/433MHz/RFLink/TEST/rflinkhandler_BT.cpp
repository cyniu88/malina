#include <gtest/gtest.h>

#include "../rflinkhandler.h"
#include "/home/pi/programowanie/iDom_server_OOP/test/iDom_TESTs-CMAKE/test_data.h"


TEST(RFLinkHandler_Class, main)
{
    thread_data test_my_data;
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.RFLinkBaudRate = "57600";
    test_server_set.RFLinkPort = "test RFLink RS232";
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;
    RFLinkHandler test_RFLink(&test_my_data);


    test_RFLink.init();
    SerialPi_set_recv_msg("msg_test;\n a=4:b=10;\n");
    std::string r = test_RFLink.sendCommandAndWaitForReceive("test command");


    EXPECT_STREQ(r.c_str(),"msg_test;");
    r = test_RFLink.sendCommandAndWaitForReceive("ok");
    EXPECT_STREQ(r.c_str()," a=4:b=10;");
}

TEST(RFLinkHandler_Class, port_does_not_exist)
{
    thread_data test_my_data;
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.RFLinkBaudRate = "57600";
    test_server_set.RFLinkPort = "/dev/fakePortRS232";
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;
    RFLinkHandler test_RFLink(&test_my_data);

    bool result =  test_RFLink.init();
    EXPECT_FALSE(result);
}


TEST(RFLinkHandler_Class, getValue)
{
    thread_data test_my_data;
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.RFLinkBaudRate = "57600";
    test_server_set.RFLinkPort = "/dev/fakePortRS232";
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;
    RFLinkHandler test_RFLink(&test_my_data);

    std::string m  = "20;90;Alecto V4;ID=557a;TEMP=0057;HUM=25;";
    EXPECT_STREQ(test_RFLink.getArgumentValueFromRFLinkMSG(m,"ID").c_str(),"557a");
    EXPECT_STREQ(test_RFLink.getArgumentValueFromRFLinkMSG(m,"TEMP").c_str(),"0057");
    EXPECT_THROW(test_RFLink.getArgumentValueFromRFLinkMSG(m,"test"),std::string);
}
