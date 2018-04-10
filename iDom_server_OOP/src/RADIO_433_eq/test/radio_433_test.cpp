#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../radio_433_eq.h"

RC_433MHz::RC_433MHz(thread_data *test_my_data){
}
void RC_433MHz::sendCode(std::string code){
}
std::string RC_433MHz::receiveCode(){
    return "test";
}

TEST(Switch_Class, getSwitchPointerVector)
{
    thread_data test_my_data;
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    RADIO_EQ_CONTAINER test_radio_cont(&test_my_data);

    test_radio_cont.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    auto v = test_radio_cont.getSwitchPointerVector();
    EXPECT_EQ(v.size(),2);
}

TEST(Switch_Class, getButtonPointerVector)
{
    thread_data test_my_data;
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    RADIO_EQ_CONTAINER test_radio_cont(&test_my_data);

    test_radio_cont.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    auto v = test_radio_cont.getButtonPointerVector();
    EXPECT_EQ(v.size(),1);
}

TEST(Switch_Class, switch_alarm_on)
{
    thread_data test_my_data;
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;
    RADIO_EQ_CONTAINER test_radio_cont(&test_my_data);

    test_radio_cont.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    RADIO_SWITCH* ptr = dynamic_cast<RADIO_SWITCH*>(test_radio_cont.getEqPointer("ALARM"));

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

TEST(Switch_Class, weatherStruct)
{
    WEATHER_STRUCT test_WS;
    EXPECT_DOUBLE_EQ(0.0, test_WS.getTemperature())<<"Tempertura zla";

    test_WS.putData("20;03;LaCrosse;ID=0506;TEMP=0137;");
    EXPECT_DOUBLE_EQ(31.1, test_WS.getTemperature())<<"Tempertura zla";
}
