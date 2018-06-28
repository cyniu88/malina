#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../radio_433_eq.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

RC_433MHz::RC_433MHz(thread_data *test_my_data){
}
void RC_433MHz::sendCode(std::string code){
    std::cout << "sendCode(): " << code << std::endl;
}
std::string RC_433MHz::receiveCode(){
    return "test";
}
class Switch_Class_fixture : public iDomTOOLS_ClassTest{

};
TEST_F(Switch_Class_fixture, getSwitchPointerVector)
{
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
}

TEST_F(Switch_Class_fixture, getButtonPointerVector)
{
    auto v = test_rec.getButtonPointerVector();
    EXPECT_EQ(v.size(),1);
}

TEST_F(Switch_Class_fixture, switch_alarm_on)
{
    RADIO_SWITCH* ptr = dynamic_cast<RADIO_SWITCH*>(test_rec.getEqPointer("ALARM"));

    EXPECT_EQ(ptr->getType(),RADIO_EQ_TYPE::SWITCH);
    puts("radio switch type");
    EXPECT_EQ(ptr->getState(),STATE::UNDEFINE);
    puts("radio switch state");
    ptr->on();
    EXPECT_EQ(ptr->getState(),STATE::ON);
    ptr->off();
    EXPECT_EQ(ptr->getState(),STATE::OFF);
    ptr->onSunset();
    EXPECT_EQ(ptr->getState(),STATE::OFF);
}

TEST_F(Switch_Class_fixture, weatherStruct)
{
    WEATHER_STRUCT test_WS;
    EXPECT_DOUBLE_EQ(0.0, test_WS.getTemperature())<<"Tempertura zla";

    test_WS.putData("20;03;LaCrosse;ID=0506;TEMP=0137;");
    EXPECT_DOUBLE_EQ(31.1, test_WS.getTemperature())<<"Tempertura zla";

    test_WS.putData("20;03;LaCrosse;ID=0506;TEMP=8130;");
    EXPECT_DOUBLE_EQ(-30.4, test_WS.getTemperature())<<"Tempertura zla";
}

TEST_F(Switch_Class_fixture, read_write_config_json)
{
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_rec.saveConfig(configPath433mhz);
    v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
}
