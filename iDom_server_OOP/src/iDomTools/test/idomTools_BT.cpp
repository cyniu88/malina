#include <gtest/gtest.h>


#include "/home/pi/programowanie/iDom_server_OOP/test/iDom_TESTs-CMAKE/test_data.h"

#include "../idomtools.h"
#include "/home/pi/programowanie/iDom_server_OOP/src/functions/functions.h"
#include "../../RADIO_433_eq/radio_433_eq.h"

void useful_F::button_interrupt(){}
void digitalWrite(int pin, int mode){}
int digitalRead(int pin){ return 0; }


std::string useful_F::send_to_arduino(thread_data *my_data,std::string d){
    return TEST_DATA::return_send_to_arduino;
}
viber_API::viber_API(){}
void viber_API::setAccessToken(std::string accessToken){}
void viber_API::setURL(std::string url){}
void viber_API::setAvatar (std::string avatar){}
std::string viber_API::sendViberMSG(std::string msg,
                                    std::string receiver,
                                    std::string senderName,
                                    std::string accessToken,
                                    std::string url){

    std::cout << "sendViberMSG() "<< msg <<" to: "<< receiver << std::endl;
    TEST_DATA::return_viber_msg = msg;
    return"";
}
std::string viber_API::sendViberPicture(std::string msg,
                                        std::string image,
                                        std::string receiver,
                                        std::string senderName,
                                        std::string accessToken ,
                                        std::string url){
    std::cout << "sendViberPicture() "<< msg <<" to: "<< receiver << std::endl;
    TEST_DATA::return_viber_msg = msg;
    return"";
}
FACEBOOK_API::FACEBOOK_API(){}
std::string FACEBOOK_API::postTxtOnWall(std::string msg, std::string accessToken ){return "";}
std::string FACEBOOK_API::postPhotoOnWall(std::string url,std::string msg ,std::string accessToken ){return "";}
void FACEBOOK_API::setAccessToken(std::string token){}

void LCD_c::set_lcd_STATE(int i){}
void LCD_c::printString(bool clear, int col, int row, std::string str){
    std::cout << "LCD_c::printString() "<< str  << std::endl;
}

std::string useful_F::httpPost(std::string url, int timeoutSeconds){

    std::cout << "url: "<< url << std::endl;
    TEST_DATA::return_httpPost_expect = "httpPost";
    return TEST_DATA::return_httpPost;
}

TEST(iDomTOOLS_Class, smog)
{
    thread_data test_my_data;
    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);

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

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    iDomTOOLS test_idomTOOLS(&test_my_data);
    // inside outside

    std::cout << "##################################### 0" <<std::endl;

    TEST_DATA::return_send_to_arduino = "20.0:-1.0;";
    test_idomTOOLS.send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("outside"),TEMPERATURE_STATE::Under);
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);
    std::cout << "##################################### 1" <<std::endl;

    TEST_DATA::return_send_to_arduino = "25.4:0.0;";
    test_idomTOOLS.send_temperature_thingSpeak();

    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("inside"),TEMPERATURE_STATE::Over);

    std::cout << "##################################### 2" <<std::endl;

    TEST_DATA::return_send_to_arduino = "21.0:1.0;";
    test_idomTOOLS.send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("outside"),TEMPERATURE_STATE::Over);
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("inside"),TEMPERATURE_STATE::Under);

    std::cout << "##################################### 3" <<std::endl;
    TEST_DATA::return_send_to_arduino = "21.0:5.0;";
    test_idomTOOLS.send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);

    std::cout << "##################################### 4" <<std::endl;
    TEST_DATA::return_send_to_arduino = "21.0:4.0;";
    test_idomTOOLS.send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);
    std::cout << "##################################### 5" <<std::endl;

    TEST_DATA::return_send_to_arduino = "31.9:11.11;";
    test_idomTOOLS.send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS.allThermometer.getLastState("inside"),TEMPERATURE_STATE::Over);
    std::cout << "##################################### 6" <<std::endl;
}

TEST(iDomTOOLS_Class, weatherAlert)
{
    std::string test_data_from_www = "    <div style=\"margin:0;padding:0;width:350px;font:0.8em Lucida,Arial,sans-seri                                                                  f;background:#FFC\">\
            <p style=\"margin:1px;padding:1px;text-align:center;background:#FF9;borde \                                                                 r:1px dotted\"><b><a href=\"http://burze.dzis.net?page=wyszukiwarka&amp;miejscowos\                                                                  c=krakow\" target=\"_blank\" style=\"color:#00E\">krakow</a></b>\
            <i>(50°03'N 19°57'E)</i>\
            </p>\
            <dl style=\"margin:1px 1px 0 1px;padding:0;cl                                                                  ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:\                                                                  center\">Zarejestrowano 54 wyładowania atmosferyczne w promieniu 300km . Najbliższe 79.76km na zachód.\
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

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);
    config test_server_set;
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_idomTOOLS.getAlert(test_data_from_www);
    EXPECT_LT(2,3);
}

TEST(iDomTOOLS_Class, send_temperature_thingSpeak){

    TEST_DATA::return_httpPost_expect = "NULL";
    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);
    config test_server_set;
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    test_server_set.TS_KEY = "keytest";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"NULL");
    test_idomTOOLS.send_temperature_thingSpeak();
    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"httpPost");
}

TEST(iDomTOOLS_Class, checkAlarm)
{
    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);
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

TEST(iDomTOOLS_Class, homeLockPlayStopMusic)
{
    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;

    blockQueue test_q;
    test_q._clearAll();
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNDEFINE);
    test_idomTOOLS.lockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);
    test_idomTOOLS.MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(),0);
    test_idomTOOLS.unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);
    test_idomTOOLS.MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
    test_idomTOOLS.lockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);
    test_idomTOOLS.MPD_stop();
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);
}

TEST(iDomTOOLS_Class, buttonPressed)
{
    std::string button433MHz_id = "01e7be";
    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_rec.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    test_my_data.main_REC = (&test_rec);
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;

    std::string pressedButtonName = test_idomTOOLS.buttonPressed(button433MHz_id);
    EXPECT_EQ(1, test_my_data.main_REC->getButtonPointerVector().size());
    EXPECT_STREQ(std::to_string(button433MHz_id).c_str(),
                 test_my_data.main_REC->getButtonPointerVector().at(0)->getID().c_str());
    EXPECT_STREQ(pressedButtonName.c_str(), "locker");

    EXPECT_THROW(test_idomTOOLS.buttonPressed(button433MHz_id+"a"),
                 std::string);
}

TEST(iDomTOOLS_Class, button433MHzPressedAction_lockerUnlock)
{
    thread_data test_my_data;

    blockQueue test_q;
    test_q._clearAll();

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;

    test_idomTOOLS.unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);

    for(auto i : {1,2,3,4,5,6}){
        test_idomTOOLS.button433MHzPressedAction("locker");
    }
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);

    EXPECT_EQ(test_q._size(),2);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
}

TEST(iDomTOOLS_Class, button433MHzPressedAction_lockerLock)
{
    thread_data test_my_data;

    blockQueue test_q;
    test_q._clearAll();

    pilot_led test_pilot_led;
    test_my_data.ptr_pilot_led = &test_pilot_led;
    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_my_data.main_REC = (&test_rec);
    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;

    //test_idomTOOLS.unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNDEFINE);

    test_idomTOOLS.button433MHzPressedAction("locker");
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
}

