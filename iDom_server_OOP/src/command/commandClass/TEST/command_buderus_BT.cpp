#include "../command_buderus.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_buderus_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    std::string test_boilerData = "{\"wWComfort\":\"Hot\",   \"wWSelTemp\":60,   \"wWDesiredTemp\":70,   \"selFlowTemp\":5,   \"selBurnPow\":0,   \"curBurnPow\":0,   \"pumpMod\":10,   \"wWCircPump\":0,   \"curFlowTemp\":30.9,   \"switchTemp\":0,"
                                  "\"boilTemp\":16.4,   \"wWActivated\":\"off\",   \"wWOnetime\":\"on\",   \"burnGas\":\"off\",   \"flameCurr\":0,   \"heatPmp\":\"on\",   \"fanWork\":\"off\",   \"ignWork\":\"off\",   \"wWCirc\":\"off\","
                                  "\"heating_temp\":50,   \"outdoorTemp\":9.99,   \"wwStorageTemp2\":62.2,   \"pump_mod_max\":100,   \"pump_mod_min\":10,   \"wWHeat\":\"off\",   \"UBAuptime\":14590,   \"burnStarts\":27,   \"burnWorkMin\":13594,   \"heatWorkMin\":13594,   \"ServiceCode\":\"0H\","
                                  "\"ServiceCodeNumber\":203}";
    std::string test_thermostatData = "  {\"currtemp\": 22.5,   \"mode\": \"manual\",  \"seltemp\": 22  } ";
    std::string test_boielrDataWrongJSON = "fake json";
    command_buderus_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_buderus> test_command_buderus;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_buderus = std::make_unique <command_buderus> ("buderus");
        test_my_data.ptr_buderus = std::make_unique<BUDERUS>();
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_buderus_Class_fixture, wrong_paramiter)
{
    test_v.push_back("buderus");
    auto ret = test_command_buderus->execute(test_v,&test_my_data);
    EXPECT_THAT( ret, testing::HasSubstr("wrong paramiter") );
}


TEST_F(command_buderus_Class_fixture, heating_active)
{
    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("heating_active");
    test_v.push_back("1");
    (void) test_command_buderus->execute(test_v,&test_my_data);
    EXPECT_TRUE(test_my_data.ptr_buderus->isHeatingActiv());

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("heating_active");
    test_v.push_back("0");
    (void) test_command_buderus->execute(test_v,&test_my_data);
    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());
}

TEST_F(command_buderus_Class_fixture, tapwater_active)
{
    EXPECT_FALSE(test_my_data.ptr_buderus->isTapWaterActiv());
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("tapwater_active");
    test_v.push_back("1");
    (void) test_command_buderus->execute(test_v,&test_my_data);
    EXPECT_TRUE(test_my_data.ptr_buderus->isTapWaterActiv());

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("tapwater_active");
    test_v.push_back("0");
    (void) test_command_buderus->execute(test_v,&test_my_data);
    EXPECT_FALSE(test_my_data.ptr_buderus->isTapWaterActiv());
}

TEST_F(command_buderus_Class_fixture, boiler_data)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(test_boilerData);
    (void)test_command_buderus->execute(test_v,&test_my_data);
    auto ret = test_my_data.ptr_buderus->getAllData();
    EXPECT_THAT(ret, ::testing::HasSubstr("13594"));
}

TEST_F(command_buderus_Class_fixture, boiler_data_wrong_json_format)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(test_boielrDataWrongJSON);
    (void)test_command_buderus->execute(test_v,&test_my_data);
    auto ret = test_my_data.iDomAlarm.showAlarm();
    std::cout << "alarmy: " << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("buderus boile_data - wrong JSON format!"));
}

TEST_F(command_buderus_Class_fixture, thermostat_data)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("thermostat_data1");
    test_v.push_back(test_thermostatData);
    (void)test_command_buderus->execute(test_v,&test_my_data);
    auto ret = test_my_data.ptr_buderus->getAllData();
    EXPECT_THAT(ret, ::testing::HasSubstr(": 22"));
}

TEST_F(command_buderus_Class_fixture, thermostat_data_wrong_json_format)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("thermostat_data1");
    test_v.push_back("fake json");
    (void)test_command_buderus->execute(test_v,&test_my_data);
    auto ret = test_my_data.iDomAlarm.showAlarm();
    std::cout << "alarmy: " << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("buderus thermostat_data1 - wrong JSON format!"));
}

TEST_F(command_buderus_Class_fixture, print)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler_data");
    test_v.push_back(test_boilerData);
    (void)test_command_buderus->execute(test_v,&test_my_data);
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("thermostat_data1");
    test_v.push_back(test_thermostatData);
    (void)test_command_buderus->execute(test_v,&test_my_data);


    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("print");
    auto ret = test_command_buderus->execute(test_v,&test_my_data);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("thermostat_data"));
    nlohmann::json checkJson(nlohmann::json::parse(ret));
    EXPECT_FALSE(checkJson["m_tapwater_active"].get<int>());
    EXPECT_FALSE(checkJson.at("m_boiler_data").at("curBurnPow").get<int>());
    EXPECT_STREQ(checkJson.at("m_boiler_data").at("wWHeat").get<std::string>().c_str(), "off");
}

TEST_F(command_buderus_Class_fixture, circle_pomp)
{
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> threadArray;

    test_my_data.main_THREAD_arr = &threadArray;

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("print");
    auto ret = test_command_buderus->execute(test_v,&test_my_data);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("\"m_circlePompCanRun\": 0"));

    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("circPomp");

    ret = test_command_buderus->execute(test_v,&test_my_data);

    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("RUN circle pomp"));
      sleep(1);
}

TEST_F(command_buderus_Class_fixture, boiler_heating)
{
    test_v.clear();
    test_v.push_back("buderus");
    test_v.push_back("boiler");
    test_v.push_back("heating");
    auto ret = test_command_buderus->execute(test_v,&test_my_data);
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
    auto ret = test_command_buderus->execute(test_v,&test_my_data);
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
    auto ret = test_command_buderus->execute(test_v,&test_my_data);
    std::cout << ret << std::endl;
    EXPECT_STREQ(ret.c_str(), "cannot convert to float");
}
