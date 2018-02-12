#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "../idomtools.h"
#include "/home/pi/programowanie/iDom_server_OOP/src/functions/functions.h"
#include "../../RADIO_433_eq/radio_433_eq.h"

void useful_F::button_interrupt(){}
void digitalWrite(int pin, int mode){}
int digitalRead(int pin){    return 0;}
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

//iDomSTATUS::iDomSTATUS(){};
//void iDomSTATUS::addObject(std::string name, STATE s){}
//STATE iDomSTATUS::getObjectState(std::string name){return STATE::UNKNOWN;}

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


TEST(iDomTOOLS_Class, weatherAlert)
{
    std::string test_data_from_www = "    <div style=\"margin:0;padding:0;width:350px;font:0.8em Lucida,Arial,sans-seri                                                                  f;background:#FFC\">\
            <p style=\"margin:1px;padding:1px;text-align:center;background:#FF9;borde \                                                                 r:1px dotted\"><b><a href=\"http://burze.dzis.net?page=wyszukiwarka&amp;miejscowos\                                                                  c=krakow\" target=\"_blank\" style=\"color:#00E\">krakow</a></b>\
            <i>(50°03'N 19°57'E)</i>\
            </p>\
            <dl style=\"margin:1px 1px 0 1px;padding:0;cl                                                                  ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:\                                                                  center\">Brak wyładowań atmosferycznych\
            w promieniu 40km\
            </dl>\
            \
            <dl style=\"margin:1px 1px 0 1px;padding:0;cl                                                                  ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:  \                                                                center\">Mróz, brak ostrzeżeń</dl>\
            \
            <dl style=\"margin:1px 1px 0 1px;padding:0;clear:both                                                                  ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  \                                                                Upał, brak ostrzeżeń</dl>\
            \
            <dl style=\"margin:1px 1px 0 1px;padding:0;clear:both                                                                  ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  \                                                                Wiatr, brak ostrzeżeń</dl>\
            \
            <dl style=\"margin:1px 1px 0 1px;padding:0;clear:both                                                                  ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  \                                                                Opady, brak ostrzeżeń</dl>\
            \
            <dl style=\"margin:1px 1px 0 1px;padding:0;clear:both                                                                  ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  \                                                                Burze, brak ostrzeżeń</dl>\
            \
            <dl style=\"margin:1px 1px 0 1px;padding:0;clear:both                                                                  ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  \                                                                Trąby powietrzne, brak ostrzeżeń</dl>\
            </div>";
            thread_data test_my_data;
    config test_server_set;
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_idomTOOLS.getAlert(test_idomTOOLS.getWeatherEvent("Krakow",40));
    //test_idomTOOLS.getAlert(test_data_from_www);
    EXPECT_LT(2,3);

}

TEST(iDomTOOLS_Class, checkAlarm)
{
    thread_data test_my_data;
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;
    MPD_info test_ptr_MPD;
    test_ptr_MPD.volume = 5;
    test_my_data.ptr_MPD_info = &test_ptr_MPD;
    RADIO_EQ* test_RS = new RADIO_SWITCH(&test_my_data,"ALARM","209888",RADIO_EQ_TYPE::SWITCH);
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    iDomTOOLS test_iDom_TOOLS(&test_my_data);
    test_my_data.main_iDomTools = &test_iDom_TOOLS;

    RADIO_EQ_CONTAINER_STUB stub_rec(&test_my_data);
    test_my_data.main_REC = (&stub_rec);
    test_my_data.alarmTime.time = Clock::getTime();
    test_my_data.alarmTime.state = STATE::ACTIVE;

    pilot_led test_pilot_led;
    test_my_data.ptr_pilot_led = &test_pilot_led;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::ACTIVE);

    for(unsigned int i = 50; i<58;++i){
        test_idomTOOLS.checkAlarm();

        EXPECT_EQ(test_my_data.alarmTime.state, STATE::WORKING);
        EXPECT_EQ(test_my_data.ptr_MPD_info->volume, i+1);
    }
    EXPECT_CALL(stub_rec, getEqPointer("ALARM")).WillRepeatedly(testing::Return(test_RS));
    test_idomTOOLS.checkAlarm();  

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::DEACTIVE);
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, 58);

    delete test_RS;
}
