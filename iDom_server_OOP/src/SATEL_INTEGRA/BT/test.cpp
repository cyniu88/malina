#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../satel_integra_handler.h"

class satel_integra_fixture: public testing::Test
{
public:
    satel_integra_fixture() = default;
protected:

    void SetUp()
    {
        std::cout << "satel_integra_fixture SetUp()" << std::endl;

    }
    void TearDown()
    {
        std::cout << "satel_integra_fixture TearDown()" << std::endl;
    }

};

TEST_F(satel_integra_fixture, main)
{
   SATEL_INTEGRA testIntegra;
   testIntegra.connectIntegra("192.168.13.156", 7094);
   std::cout << testIntegra.getIntegraInfo();
   testIntegra.checkIntegraOut();
   EXPECT_FALSE(testIntegra.isAlarmArmed());
   testIntegra.disconnectIntegra();
}
#include <bitset>
TEST_F(satel_integra_fixture, checkIntegraOut)
{
    struct CONFIG_JSON test_config;
    test_config._satel_integra.host = "192.168.13.156";
    test_config._satel_integra.port = 7094;
    test_config._satel_integra.pin = "1234";
    thread_data test_threadData;
    test_threadData.server_settings = &test_config;
    SATEL_INTEGRA_HANDLER testIntegra(&test_threadData);

    testIntegra.checkSatel();
}
