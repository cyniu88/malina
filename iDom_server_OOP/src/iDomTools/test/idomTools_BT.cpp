#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../idomtools.h"
#include "../../src/functions/functions.h"

void useful_F::button_interrupt(){}
//
std::string useful_F::send_to_arduino(thread_data *my_data,std::string d){
    static int i = 0;
    i++;
    switch(i){
    case 1:
        puts("CASE 1");
        return "-1:-1;";
        break;
    case 2:
        puts("CASE 2");
        return "0:0;";
    case 3:
        puts("CASE 3");
        return "1:1;";
    case 4:
        puts("CASE 4");
        return "2:2;";
    default:
        puts("CASE default");
        return "13:13;";
    }

    return "13:13;";
}
viber_API::viber_API(){}
void viber_API::setAccessToken(std::string accessToken){}
void viber_API::setURL(std::string url){}
void viber_API::setAvatar (std::string avatar){}
std::string viber_API::sendViberMSG(std::string msg,
                                    std::string receiver,
                                    std::string senderName,
                                    std::string accessToken,
                                    std::string url){return"";}
std::string viber_API::sendViberPicture(std::string msg,
                                        std::string image,
                                        std::string receiver,
                                        std::string senderName,
                                        std::string accessToken ,
                                        std::string url){return"";}
FACEBOOK_API::FACEBOOK_API(){}
std::string FACEBOOK_API::postTxtOnWall(std::string msg, std::string accessToken ){return "";}
std::string FACEBOOK_API::postPhotoOnWall(std::string url,std::string msg ,std::string accessToken ){return "";}
void FACEBOOK_API::setAccessToken(std::string token){}

void LCD_c::set_lcd_STATE(int i){}
void LCD_c::printString(bool clear, int col, int row, std::string str){}

iDomSTATUS::iDomSTATUS(){};
void iDomSTATUS::addObject(std::string name, STATE s){}
STATE iDomSTATUS::getObjectState(std::string name){return STATE::UNKNOWN;}

TEST(iDomTOOLS_Class, smog)
{
    thread_data test_my_data;
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    std::string smog = test_idomTOOLS.getSmog();
    puts(smog.c_str());
    puts(" smog");
    ASSERT_GE(smog.size(),2);

    unsigned int smog_int = std::stoi(smog);
    ASSERT_GT(smog_int,1);
    ASSERT_LT(smog_int,1000);
}

TEST(iDomTOOLS_Class, hasTemperatureChange)
{
    thread_data test_my_data;
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    TEMPERATURE_STATE s = test_idomTOOLS.hasTemperatureChange("inside",0,0.5);

    test_idomTOOLS.send_temperature_thingSpeak();
    s = test_idomTOOLS.hasTemperatureChange("inside",0,0.5);
    EXPECT_EQ(s,TEMPERATURE_STATE::Under);

    test_idomTOOLS.send_temperature_thingSpeak();
    s = test_idomTOOLS.hasTemperatureChange("inside",0,0.5);
    EXPECT_EQ(s,TEMPERATURE_STATE::NoChanges);

    test_idomTOOLS.send_temperature_thingSpeak();
    s = test_idomTOOLS.hasTemperatureChange("inside",0,0.5);
    EXPECT_EQ(s,TEMPERATURE_STATE::Over);

    test_idomTOOLS.send_temperature_thingSpeak();
    s = test_idomTOOLS.hasTemperatureChange("inside",0,0.5);
    EXPECT_EQ(s,TEMPERATURE_STATE::NoChanges);
}
