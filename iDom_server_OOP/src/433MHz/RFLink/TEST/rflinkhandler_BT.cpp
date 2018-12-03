#include <gtest/gtest.h>

#include "../rflinkhandler.h"
#include "test_data.h"

class RFLinkHandler_Class_fixture : public ::testing::Test
{
public:
    thread_data test_my_data;
    config test_server_set;
    iDomSTATUS test_status;
    std::unique_ptr<RFLinkHandler> test_RFLink;

    void SetUp() final
    {
        test_server_set.TS_KEY = "key test";
        test_server_set.RFLinkBaudRate = "57600";
        test_server_set.RFLinkPort = "/dev/fakePortRS232";
        test_my_data.server_settings = &test_server_set;
        test_my_data.main_iDomStatus = &test_status;
        test_RFLink = std::make_unique<RFLinkHandler>(&test_my_data);
    }
    void TearDown() final
    {
    }
};

TEST_F(RFLinkHandler_Class_fixture, sendCommandAndWaitForReceive)
{
    test_RFLink->init();
    std::string k("msg_test;\n a=4:b=10;\n");
    SerialPi_set_recv_msg(k);
    std::string r = test_RFLink->sendCommandAndWaitForReceive("test command");

    EXPECT_STREQ(r.c_str(),"msg_test;");
    r = test_RFLink->sendCommandAndWaitForReceive("ok");
    EXPECT_STREQ(r.c_str()," a=4:b=10;");
}

TEST_F(RFLinkHandler_Class_fixture, port_does_not_exist)
{
    bool result =  test_RFLink->init();
    EXPECT_FALSE(result);
}

TEST_F(RFLinkHandler_Class_fixture, port_exist)
{
    test_server_set.RFLinkPort = "/dev/tty0";
    bool result =  test_RFLink->init();
    EXPECT_TRUE(result);
}

TEST_F(RFLinkHandler_Class_fixture, getValue)
{
    std::string m  = "20;90;Alecto V4;ID=557a;TEMP=0057;HUM=25;";
    EXPECT_STREQ(test_RFLink->getArgumentValueFromRFLinkMSG(m,"ID").c_str(),"557a");
    EXPECT_STREQ(test_RFLink->getArgumentValueFromRFLinkMSG(m,"TEMP").c_str(),"0057");
    EXPECT_THROW(test_RFLink->getArgumentValueFromRFLinkMSG(m,"test"),std::string);
}

TEST_F(RFLinkHandler_Class_fixture, readAndFlush)
{
    SerialPi_set_recv_msg("doTest;\n");
    std::string retStr = test_RFLink->readFromRS232();
    EXPECT_STREQ(retStr.c_str(), "doTest;");

    SerialPi_set_recv_msg("doTest2;\n");
    test_RFLink->flush();
    retStr = test_RFLink->readFromRS232();
    EXPECT_EQ(retStr.size(), 0);
}
