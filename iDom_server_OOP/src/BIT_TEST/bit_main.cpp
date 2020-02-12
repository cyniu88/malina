#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>

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
    void start_iDomServer();
    static void iDomServerStub();
    std::unique_ptr<TASKER> bit_Tasker;
};

void bit_fixture::start_iDomServer()
{
    std::thread t(&iDomServerStub);
    t.join();
}

void bit_fixture::iDomServerStub()
{
    std::cout << "Działa!!!!" << std::endl;
}

TEST_F(bit_fixture, heandle_command){
    start_iDomServer();
}

TEST_F(bit_fixture, buderus_mqtt_command_from_boiler){

    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","1");
    bit_Tasker->runTasker();
    EXPECT_TRUE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","0");
    bit_Tasker->runTasker();
    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","1");
    bit_Tasker->runTasker();
    EXPECT_TRUE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","offline");
    bit_Tasker->runTasker();
    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());
    /////////////////////////////////  boiler data //////////////////////////////////////////////////////
    std::string test_boilerData = "{\"wWComfort\":\"Hot\",   \"wWSelTemp\":60,   \"wWDesiredTemp\":70,   \"selFlowTemp\":5,   \"selBurnPow\":0,   \"curBurnPow\":0,   \"pumpMod\":10,   \"wWCircPump\":0,   \"curFlowTemp\":30.9,   \"switchTemp\":0,"
                                  "\"boilTemp\":16.4,   \"wWActivated\":\"off\",   \"wWOnetime\":\"on\",   \"burnGas\":\"off\",   \"flameCurr\":0,   \"heatPmp\":\"on\",   \"fanWork\":\"off\",   \"ignWork\":\"off\",   \"wWCirc\":\"off\","
                                  "\"heating_temp\":50,   \"pump_mod_max\":100,   \"pump_mod_min\":10,   \"wWHeat\":\"off\",   \"UBAuptime\":14590,   \"burnStarts\":27,   \"burnWorkMin\":13594,   \"heatWorkMin\":13594,   \"ServiceCode\":\"0H\","
                                  "\"ServiceCodeNumber\":203}";

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/boiler_data",test_boilerData);
    bit_Tasker->runTasker();
    auto ret = test_my_data.ptr_buderus->dump();
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("13594"));
}

