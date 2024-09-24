#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_buderus.h"
#include "../../iDom_server_OOP/src/iDomTools/mock/iDomToolsMock.h"
#include "../../iDom_server_OOP/src/functions/functions.h"
#include "../../iDom_server_OOP/src/buderus/buderus.h"

class command_buderus_Class_fixture : public testing::Test
{
public:
    std::string test_boilerData = R"({"wWComfort":"Hot",  "burnGas":"on", "wWSelTemp":60,   "wWDesiredTemp":70,   "selFlowTemp":5,   "selBurnPow":0,   "curBurnPow":0,   "pumpMod":10, "wWCirc":"on", "wWCircPump":"off",   "curFlowTemp":30.9,   "switchTemp":0,)"
                                  R"("boilTemp":16.4,   "wWActivated":"off",   "wWOnetime":"on",    "flameCurr":0,   "heatPmp":"on",   "fanWork":"off",   "ignWork":"off",   )"
                                  R"("heating_temp":50,   "outdoorTemp":9.99,   "wwStorageTemp2":62.2,   "pump_mod_max":100,   "pump_mod_min":10,   "wWHeat":"off",   "UBAuptime":14590,   "burnStarts":27,   "burnWorkMin":13594,   "heatWorkMin":13594,   "ServiceCode":"0H",)"
                                  R"("ServiceCodeNumber":203})";
    std::string test_thermostatData = R"({"hc1": {"currtemp": 22.5,"mode": "manual", "seltemp": 22  } })";
    std::string test_boielrDataWrongJSON = "fake json";
    command_buderus_Class_fixture() : test_command_buderus(std::make_unique<command_buderus>("buderus")),
        main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.ptr_buderus = std::make_shared<BUDERUS>();
        test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("cyniu");
        test_context.main_iDomTools = main_iDomTools;
        test_context.server_settings = &test_server_set;
        test_context.server_settings->_fb_viber.viberSender = "test sender";
        test_context.server_settings->_fb_viber.viberReceiver = {"R1","R2"};
        test_context.main_Rs232 = std::make_unique<SerialPi>("cyniu");
        useful_F::myStaticCtx = &test_context;
    }

protected:
    std::unique_ptr<command_buderus> test_command_buderus;
    thread_context test_context;
    std::vector<std::string> test_v;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    CONFIG_JSON test_server_set;
};

TEST_F(command_buderus_Class_fixture, wrong_paramiter)
{
    test_v.push_back("buderus");
    auto ret = test_command_buderus->execute(test_v, &test_context);
    EXPECT_THAT( ret, testing::HasSubstr("wrong paramiter") );
}


TEST_F(command_buderus_Class_fixture, heating_active)
{
    EXPECT_FALSE(test_context.ptr_buderus->isHeatingActiv());
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("heating_active");
    test_v.push_back("1");
    (void) test_command_buderus->execute(test_v, &test_context);
    EXPECT_TRUE(test_context.ptr_buderus->isHeatingActiv());

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("heating_active");
    test_v.push_back("0");
    (void) test_command_buderus->execute(test_v,&test_context);
    EXPECT_FALSE(test_context.ptr_buderus->isHeatingActiv());
}

TEST_F(command_buderus_Class_fixture, tapwater_active)
{
    EXPECT_FALSE(test_context.ptr_buderus->isTapWaterActiv());
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("tapwater_active");
    test_v.push_back("1");
    (void) test_command_buderus->execute(test_v,&test_context);
    EXPECT_TRUE(test_context.ptr_buderus->isTapWaterActiv());

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("tapwater_active");
    test_v.push_back("0");
    (void) test_command_buderus->execute(test_v,&test_context);
    EXPECT_FALSE(test_context.ptr_buderus->isTapWaterActiv());
}

TEST_F(command_buderus_Class_fixture, boiler_data)
{
    EXPECT_CALL(*main_iDomTools.get(), sendViberMsg(testing::_,testing::_,testing::_,testing::_,testing::_));

    EXPECT_FALSE(test_context.ptr_buderus->isHeatingActiv());
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(test_boilerData);
    (void)test_command_buderus->execute(test_v,&test_context);
    auto ret = test_context.ptr_buderus->getAllData();
    EXPECT_THAT(ret, ::testing::HasSubstr("13594"));
    EXPECT_TRUE(test_context.ptr_buderus->isHeatingActiv());
}

TEST_F(command_buderus_Class_fixture, boiler_data_wrong_json_format)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(test_boielrDataWrongJSON);
    (void)test_command_buderus->execute(test_v,&test_context);
    auto ret = test_context.iDomAlarm.showAlarm();
    std::cout << "alarmy: " << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("buderus boile_data - wrong JSON format!"));
}

TEST_F(command_buderus_Class_fixture, thermostat_data)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("thermostat_data");
    test_v.push_back(test_thermostatData);
    (void)test_command_buderus->execute(test_v,&test_context);
    auto ret = test_context.ptr_buderus->getAllData();
    EXPECT_THAT(ret, ::testing::HasSubstr(": 22"));
}

TEST_F(command_buderus_Class_fixture, thermostat_data_wrong_json_format)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("thermostat_data");
    test_v.push_back("fake json");
    (void)test_command_buderus->execute(test_v,&test_context);
    auto ret = test_context.iDomAlarm.showAlarm();
    std::cout << "alarmy: " << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("buderus thermostat_data - wrong JSON format!"));
}

TEST_F(command_buderus_Class_fixture, print)
{
    EXPECT_CALL(*main_iDomTools.get(), sendViberMsg(testing::_,testing::_,testing::_,testing::_,testing::_));
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(test_boilerData);
    (void)test_command_buderus->execute(test_v,&test_context);
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("thermostat_data");
    test_v.push_back(test_thermostatData);
    (void)test_command_buderus->execute(test_v,&test_context);


    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("print");
    auto ret = test_command_buderus->execute(test_v,&test_context);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("thermostat_data"));
    nlohmann::json checkJson(nlohmann::json::parse(ret));
    EXPECT_FALSE(checkJson["m_tapwater_active"].get<int>());
    EXPECT_FALSE(checkJson.at("m_boiler_data").at("curBurnPow").get<int>());
    EXPECT_STREQ(checkJson.at("m_boiler_data").at("wWHeat").get<std::string>().c_str(), "off");
}

TEST_F(command_buderus_Class_fixture, circle_pomp)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("print");
    auto ret = test_command_buderus->execute(test_v,&test_context);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr(R"("m_circlePompCanRun": 0)"));

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("circPomp");

    ret = test_command_buderus->execute(test_v,&test_context);

    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("RUN circle pomp"));
}

TEST_F(command_buderus_Class_fixture, boiler_heating)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler");
    test_v.push_back("heating");
    auto ret = test_command_buderus->execute(test_v,&test_context);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("Start heating boiler"));
}

TEST_F(command_buderus_Class_fixture, set_desired_temp)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("set");
    test_v.push_back("temp");
    test_v.push_back("2.2");
    auto ret = test_command_buderus->execute(test_v,&test_context);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("2.2"));
}

TEST_F(command_buderus_Class_fixture, set_desired_temp_fake)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("set");
    test_v.push_back("temp");
    test_v.push_back("fake");
    auto ret = test_command_buderus->execute(test_v,&test_context);
    std::cout << ret << std::endl;
    EXPECT_STREQ(ret.c_str(), "cannot convert to float");
}

TEST_F(command_buderus_Class_fixture, circlePump_handling) {

    EXPECT_CALL(*main_iDomTools.get(), sendViberMsg(testing::_,testing::_,testing::_,testing::_,testing::_)).Times(2);
    EXPECT_THAT(test_context.ptr_buderus->getCirclePumpState(),
                STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(test_boilerData);
    (void)test_command_buderus->execute(test_v,&test_context);
    EXPECT_THAT(test_context.ptr_buderus->getCirclePumpState(),
                STATE::ON);

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(R"({ "wWCirc":"on", "burnGas":"on", "curFlowTemp":44.4, "outdoorTemp":9.99, "wwStorageTemp2":62.2 })");
    (void)test_command_buderus->execute(test_v,&test_context);
    EXPECT_THAT(test_context.ptr_buderus->getCirclePumpState(),
                STATE::ON);

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(R"({ "wWCirc":"off", "burnGas":"on", "curFlowTemp":44.4, "outdoorTemp":9.99, "wwStorageTemp2":62.2 })");
    (void)test_command_buderus->execute(test_v,&test_context);
    EXPECT_THAT(test_context.ptr_buderus->getCirclePumpState(),
                STATE::OFF);

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(R"({ "wWCirc":"off", "burnGas":"on", "curFlowTemp":44.4, "outdoorTemp":9.99, "wwStorageTemp2":62.2 })");
    (void)test_command_buderus->execute(test_v,&test_context);
    EXPECT_THAT(test_context.ptr_buderus->getCirclePumpState(),
                STATE::OFF);
}
