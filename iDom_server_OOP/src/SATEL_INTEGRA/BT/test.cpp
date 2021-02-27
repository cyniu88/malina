#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../satel_integra.h"

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
   testIntegra.isAlarmArmed();
   testIntegra.disconnectIntegra();
}
