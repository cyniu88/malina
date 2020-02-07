#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../iDomTools/test/iDomTools_fixture.h"
#include "../TASKER/tasker.h"

class bit_fixture : public iDomTOOLS_ClassTest
{
protected:
    void SetUp()
    {
        bit_Tasker = std::make_unique<TASKER>(&test_my_data);
        test_my_data.mqttHandler = std::make_unique<MQTT_mosquitto>("cyniu-BIT");
        test_my_data.ptr_buderus = std::make_unique<BUDERUS>();
    }
    void TearDown()
    {

    }
public:

    std::unique_ptr<TASKER> bit_Tasker;
};

TEST_F(bit_fixture, main){

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/tapwater_active","1");
    bit_Tasker->runTasker();
}
