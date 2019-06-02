#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <curl/curl.h>

#include "iDomTools_fixture.h"
#include "../mqtt_mosquitto/MQTT_mosquitto/mqtt.h"

TEST_F(iDomTOOLS_ClassTest, smog)
{
    std::string smog = test_idomTOOLS->getSmog();
    puts(smog.c_str());
    puts(" smog");
    ASSERT_GE(smog.size(),1);

    unsigned int smog_int = std::stoi(smog);
    ASSERT_GT(smog_int,1);
    ASSERT_LT(smog_int,1000);
}

TEST_F(iDomTOOLS_ClassTest, hasTemperatureChange)
{
    std::cout << "##################################### 0" <<std::endl;

    TEST_DATA::return_send_to_arduino = "20.0:-1.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("outside"),TEMPERATURE_STATE::Under);
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);
    std::cout << "##################################### 1" <<std::endl;

    TEST_DATA::return_send_to_arduino = "25.4:0.0;";
    test_idomTOOLS->send_temperature_thingSpeak();

    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("inside"),TEMPERATURE_STATE::Over);

    std::cout << "##################################### 2" <<std::endl;

    TEST_DATA::return_send_to_arduino = "21.0:1.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("outside"),TEMPERATURE_STATE::Over);
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("inside"),TEMPERATURE_STATE::Under);

    std::cout << "##################################### 3" <<std::endl;
    TEST_DATA::return_send_to_arduino = "21.0:5.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);

    std::cout << "##################################### 4" <<std::endl;
    TEST_DATA::return_send_to_arduino = "21.0:4.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);
    std::cout << "##################################### 5" <<std::endl;

    TEST_DATA::return_send_to_arduino = "31.9:11.11;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->m_allThermometer.getLastState("inside"),TEMPERATURE_STATE::Over);
    std::cout << "##################################### 6" <<std::endl;

    ////// getThermoStats

    std::cout << "WYNIK: " << test_idomTOOLS->getThermoStats("inside") <<std::endl;
}

TEST_F(iDomTOOLS_ClassTest, weatherAlert)
{
    std::string test_data_from_www = " <div style=\"margin:0;padding:0;width:350px;font:0.8em Lucida,Arial,sans-seri f;background:#FFC\">"
                                     "<p style=\"margin:1px;padding:1px;text-align:center;background:#FF9;borde \\ r:1px dotted\"><b><a href=\"http://burze.dzis.net?page=wyszukiwarka&amp;miejscowos\\ c=krakow\" target=\"_blank\" style=\"color:#00E\">krakow</a></b>"
                                     "<i>(50°03'N 19°57'E)</i> </p>"
                                     "<dl style=\"margin:1px 1px 0 1px;padding:0;cl ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:\\ center\">Zarejestrowano 54 wyładowania atmosferyczne w promieniu 300km . Najbliższe 79.76km na zachód."
                                     "</dl> <dl style=\"margin:1px 1px 0 1px;padding:0;cl ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align: \\ center\">Mróz, brak ostrzeżeń</dl>"
                                     "<dl style=\"margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  Upał, brak ostrzeżeń</dl>"
                                     "<dl style=\"margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  Wiatr, brak ostrzeżeń</dl>"
                                     "<dl style=\"margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  Opady, brak ostrzeżeń</dl>"
                                     "<dl style=\"margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  Burze, brak ostrzeżeń</dl>"
                                     "<dl style=\"margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center\">  Trąby powietrzne, brak ostrzeżeń</dl></div>";

    std::vector<WEATHER_ALER> test_WA;
    test_WA =  test_idomTOOLS->getAlert(test_data_from_www);
    EXPECT_EQ(1,test_WA.size()) << "ZŁY ROZMIAR VEKTORA WA";
    std::cout << std::endl << " ****************** " << std::endl <<
    iDom_API::getDump() << std::endl << " ****************** " << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, send_temperature_thingSpeak){

    TEST_DATA::return_send_to_arduino = "-2.3:-2";
    TEST_DATA::return_httpPost_expect = "NULL";
    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"NULL");
    test_idomTOOLS->send_temperature_thingSpeak();
    std::cout << "DATA: "<< TEST_DATA::return_httpPost_expect <<std::endl;
    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"httpPost");
}

TEST_F(iDomTOOLS_ClassTest, checkAlarm)
{
    blockQueue test_q;
    unsigned int fromVol = 48;
    unsigned int  toVol = 57;

    ///////////////////////////////////// to save
    test_status.setObjectState("house",STATE::UNLOCK);
    test_status.setObjectState("music", STATE::PLAY);
    test_status.setObjectState("speakers", STATE::ON);
    test_my_data.idom_all_state.houseState = STATE::LOCK;

    test_status.setObjectState("listwa",STATE::ON);

    test_alarmTime.time = Clock::getTime();
    test_alarmTime.state = STATE::ACTIVE;
    test_alarmTime.toVolume = 58;
    test_alarmTime.fromVolume = 48;
    test_status.setObjectState("alarm", test_alarmTime.state);
    test_my_data.alarmTime = test_alarmTime;
    useful_F::myStaticData = &test_my_data;

    //////////////////////////////////////////////////////////////

    test_my_data.alarmTime.time = Clock::getTime();
    test_my_data.alarmTime.state = STATE::ACTIVE;

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::ACTIVE);

    for(unsigned int i = fromVol; i<toVol; ++i)
    {
        test_idomTOOLS->checkAlarm();
        test_q._get();
        EXPECT_EQ(test_my_data.alarmTime.state, STATE::WORKING)<< "zły stan w for " << i<< " "<< toVol;
        EXPECT_EQ(test_my_data.ptr_MPD_info->volume, i+1) << "zły poziom glosnosci w for";
    }

    test_idomTOOLS->checkAlarm();

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::DEACTIVE) << "nie jest STATE::DEACTIVE";
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, toVol)<< "nie inkrementowane?";

}

TEST_F(iDomTOOLS_ClassTest, homeLockPlayStopMusic)
{
    ///////////////////////////////////// to save
    test_status.setObjectState("house",STATE::UNDEFINE);
    test_status.setObjectState("music", STATE::PLAY);
    test_status.setObjectState("speakers", STATE::ON);
    test_my_data.idom_all_state.houseState = STATE::LOCK;

    test_status.setObjectState("listwa",STATE::ON);

    test_alarmTime.time = Clock::getTime();
    test_alarmTime.state = STATE::ACTIVE;
    test_status.setObjectState("alarm", test_alarmTime.state);

    blockQueue test_q;
    test_q._clearAll();
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNDEFINE);
    test_idomTOOLS->lockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);
    test_idomTOOLS->MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(),0);
    test_idomTOOLS->unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);
    test_idomTOOLS->MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
    test_idomTOOLS->lockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);
    test_idomTOOLS->MPD_stop();
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);
    std::string returnedString = test_status.getAllObjectsStateString();
    EXPECT_THAT(returnedString, testing::HasSubstr("LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, buttonPressed)
{
    std::string button433MHz_id = "01e7be";
    std::string pressedButtonName = test_idomTOOLS->buttonPressed(button433MHz_id);
    EXPECT_EQ(3, test_my_data.main_REC->getButtonPointerVector().size());
    EXPECT_STREQ(std::to_string(button433MHz_id).c_str(),
                 test_my_data.main_REC->getButtonPointerVector().at(1)->getID().c_str());
    EXPECT_STREQ(pressedButtonName.c_str(), "locker-main");

    EXPECT_THROW(test_idomTOOLS->buttonPressed(button433MHz_id+"a"),
                 std::string);
}

TEST_F(iDomTOOLS_ClassTest, button433MHzPressedAction_lockerUnlock)
{
    blockQueue test_q;
    test_q._clearAll();

    test_idomTOOLS->unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);

    for(auto i =0 ; i < 3; ++i){
        test_idomTOOLS->button433MHzPressedAction("locker-main");
    }
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);

}

TEST_F(iDomTOOLS_ClassTest, button433MHzPressedAction_lockerLock)
{
    blockQueue test_q;
    test_q._clearAll();

    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNDEFINE) << "nie jest UNDEFINED";

    test_idomTOOLS->button433MHzPressedAction("locker-main");
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK)<< "nie jest UNLOCK";

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
}

TEST_F(iDomTOOLS_ClassTest, button433MHzPressedAction_locker_mainLock_locker_2unlock)
{
    blockQueue test_q;
    test_q._clearAll();

    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNDEFINE) << "nie jest UNDEFINED";

    test_idomTOOLS->button433MHzPressedAction("locker-main");
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK)<< "nie jest UNLOCK";

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);

    test_idomTOOLS->button433MHzPressedAction("locker-2");
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK)<< "nie jest UNLOCK";

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
}

TEST_F(iDomTOOLS_ClassTest, testCPU_Load)
{
    std::cout <<"TEST LOAD" << std::endl;
    std::cout << test_idomTOOLS->getSystemInfo() << std::endl;
}
TEST_F(iDomTOOLS_ClassTest, stringToCardinalDirectionsEnum)
{
    EXPECT_EQ(CARDINAL_DIRECTIONS::stringToCardinalDirectionsEnum("NWWA"),
              CARDINAL_DIRECTIONS::CARDINAL_DIRECTIONS_ENUM::ERROR);
    EXPECT_EQ(CARDINAL_DIRECTIONS::stringToCardinalDirectionsEnum("N"),
              CARDINAL_DIRECTIONS::CARDINAL_DIRECTIONS_ENUM::N);
}

TEST_F(iDomTOOLS_ClassTest, cardinalDirectionsEnumToString)
{
    EXPECT_STREQ( CARDINAL_DIRECTIONS::cardinalDirectionsEnumToString(CARDINAL_DIRECTIONS::CARDINAL_DIRECTIONS_ENUM::ERROR).c_str(),
                  "UNKNOWN DIRECTION");
    EXPECT_STREQ( CARDINAL_DIRECTIONS::cardinalDirectionsEnumToString(CARDINAL_DIRECTIONS::CARDINAL_DIRECTIONS_ENUM::ESE).c_str(),
                  "ESE");
}

TEST_F(iDomTOOLS_ClassTest, saveState_readState)
{
    test_status.setObjectState("house",STATE::UNLOCK);
    //////////////////// mpd
    test_status.setObjectState("music", STATE::PLAY);
    test_status.setObjectState("speakers", STATE::ON);
    test_my_data.idom_all_state.houseState = STATE::LOCK;

    test_status.setObjectState("listwa",STATE::ON);
    test_alarmTime.time = Clock::getTime();
    test_alarmTime.state = STATE::ACTIVE;
    test_alarmTime.fromVolume = 0;
    test_alarmTime.toVolume = 100;
    test_alarmTime.radioID = 44;
    test_my_data.alarmTime = test_alarmTime;
    test_status.setObjectState("alarm", test_alarmTime.state);

    test_idomTOOLS->saveState_iDom();

    nlohmann::json testJson;
    std::ifstream i(test_server_set._server.saveFilePath);
    i >> testJson;
    EXPECT_STREQ(test_status.getObjectStateString("music").c_str(),
                 testJson.at("MPD").at("music").get<std::string>().c_str() );
    EXPECT_STREQ((test_status.getObjectStateString("alarm")).c_str(),
                 testJson.at("ALARM").at("alarm").get<std::string>().c_str() );
    EXPECT_EQ(test_alarmTime.radioID,
              testJson.at("ALARM").at("radioID").get<int>() );
    EXPECT_EQ(test_alarmTime.fromVolume,
              testJson.at("ALARM").at("fromVolume").get<int>() );
    EXPECT_EQ(test_alarmTime.toVolume,
              testJson.at("ALARM").at("toVolume").get<int>() );

    ////////////////////////////////// read
    iDom_SAVE_STATE info(test_my_data.server_settings->_server.saveFilePath);
    test_idomTOOLS->readState_iDom(info.read());
    EXPECT_EQ(test_my_data.alarmTime.state,STATE::ACTIVE);

    test_my_data.server_settings->_server.saveFilePath = "null";
    EXPECT_NO_THROW(test_idomTOOLS->readState_iDom(info.read()));
}

TEST_F(iDomTOOLS_ClassTest, getLightningStruct)
{
    LIGHTNING test_lightning;
    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning);

    test_idomTOOLS->setLightningStruct(test_struct);

    bool test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_TRUE(test_result);

    auto test_alert_info = test_idomTOOLS->getLightningStruct();
    EXPECT_EQ(test_alert_info.timestamp,210);
}
TEST_F(iDomTOOLS_ClassTest, checkLightning)
{
    test_my_data.server_settings->_server.lightningApiURL = "http://cyniu88.no-ip.pl/test/json/lightning.json";
    test_idomTOOLS->checkLightning();
    auto test_alert_info = test_idomTOOLS->getLightningStruct();
    EXPECT_EQ(test_alert_info.timestamp,210);
}

TEST_F(iDomTOOLS_ClassTest, updateTemperatureStats)
{
    TEST_DATA::return_send_to_arduino = "12:12";
    test_idomTOOLS->updateTemperatureStats();
    TEST_DATA::return_send_to_arduino = "16:16";
    test_idomTOOLS->updateTemperatureStats();

    ////////////// maleje na mieskzaniu
    TEST_DATA::return_send_to_arduino = "12:16";
    test_idomTOOLS->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura maleje"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("mieszkaniu"));

    ////////////// maleje na polu
    TEST_DATA::return_send_to_arduino = "12:12";
    test_idomTOOLS->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura maleje"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("polu"));

    ////////////// rośnie na mieskzaniu
    TEST_DATA::return_send_to_arduino = "17:12";
    test_idomTOOLS->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura rośnie"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("mieszkaniu"));

    ////////////// rośnie na polu
    TEST_DATA::return_send_to_arduino = "17:17";
    test_idomTOOLS->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura rośnie"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("polu"));
}

TEST_F(iDomTOOLS_ClassTest, speakersON_OFF)
{
    EXPECT_EQ(test_status.getObjectState("speakers"), STATE::OFF);
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_idomTOOLS->turnOnSpeakers();
    EXPECT_EQ(test_status.getObjectState("speakers"), STATE::ON);
    test_idomTOOLS->turnOffSpeakers();
    EXPECT_EQ(test_status.getObjectState("speakers"), STATE::OFF);
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_idomTOOLS->turnOnSpeakers();
    EXPECT_EQ(test_status.getObjectState("speakers"), STATE::OFF);
    std::string retStr = useful_F::myStaticData->myEventHandler.run("speakers")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("speakers can not start due to home state: LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, printerON_OFF)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_idomTOOLS->turnOnPrinter();
    EXPECT_EQ(test_status.getObjectState("printer"), STATE::ON);
    test_idomTOOLS->turnOffPrinter();
    EXPECT_EQ(test_status.getObjectState("printer"), STATE::OFF);
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_idomTOOLS->turnOnPrinter();
    EXPECT_EQ(test_status.getObjectState("printer"), STATE::OFF);
    std::string retStr = useful_F::myStaticData->myEventHandler.run("230V")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("Printer can not start due to home state: LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, getPinState)
{
    setReturnPinState(0);
    EXPECT_EQ(test_idomTOOLS->getPinState(0), PIN_STATE::LOW_STATE);
    setReturnPinState(1);
    EXPECT_EQ(test_idomTOOLS->getPinState(0), PIN_STATE::HIGH_STATE);
    setReturnPinState(4);
    EXPECT_EQ(test_idomTOOLS->getPinState(0), PIN_STATE::UNKNOWN_STATE);
}

TEST_F(iDomTOOLS_ClassTest, turnOnOffPrinter)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_status.setObjectState("printer",STATE::ON);
    EXPECT_EQ(test_status.getObjectState("printer"), STATE::ON);
    setReturnPinState(1);
    puts("off printer");
    test_idomTOOLS->turnOnOffPrinter();
    EXPECT_EQ(test_status.getObjectState("printer"), STATE::OFF);
    setReturnPinState(0);
    puts("on printer");
    test_idomTOOLS->turnOnOffPrinter();
    EXPECT_EQ(test_status.getObjectState("printer"), STATE::ON);
    setReturnPinState(4);
    test_idomTOOLS->turnOnOffPrinter();
    EXPECT_EQ(test_status.getObjectState("printer"), STATE::ON);
}

TEST_F(iDomTOOLS_ClassTest, turn_On_Off_433MHzSwitch)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    EXPECT_EQ(test_status.getObjectState("B"),STATE::UNKNOWN);
    test_idomTOOLS->turnOn433MHzSwitch("B");
    EXPECT_EQ(test_status.getObjectState("B"),STATE::ON);
    test_idomTOOLS->turnOff433MHzSwitch("B");
    EXPECT_EQ(test_status.getObjectState("B"),STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, turn_On_Off_fake_433MHzSwitch)
{
    EXPECT_NO_THROW(test_idomTOOLS->turnOn433MHzSwitch("B-fake"));
    EXPECT_NO_THROW(test_idomTOOLS->turnOff433MHzSwitch("B-fake"));
    EXPECT_NO_THROW(test_idomTOOLS->turnOnOff433MHzSwitch("B-fake"));
}

TEST_F(iDomTOOLS_ClassTest, turnOnOff433MHzSwitch)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::UNKNOWN);
    test_my_data.main_iDomStatus->setObjectState("B",STATE::ON);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::ON);
    test_idomTOOLS->turnOnOff433MHzSwitch("B");
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::OFF);
    test_idomTOOLS->turnOnOff433MHzSwitch("B");
    EXPECT_EQ(test_status.getObjectState("B"),STATE::ON);

}

TEST_F(iDomTOOLS_ClassTest, runOnSunset)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_idomTOOLS->runOnSunset();
    std::string retStr = useful_F::myStaticData->myEventHandler.run("iDom")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("433MHz can not start due to home state: LOCK"));

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    auto ptr = static_cast<RADIO_SWITCH*>(test_my_data.main_REC->getEqPointer("B"));
    ptr->m_state = STATE::ON;
    test_idomTOOLS->runOnSunset();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::OFF);

    RADIO_EQ_CONFIG cfg;
    cfg.sunset = "on";
    cfg.sunrise = "off";

    ptr->setCode(cfg);
    ptr->m_state = STATE::OFF;
    test_idomTOOLS->runOnSunset();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::ON);
}

TEST_F(iDomTOOLS_ClassTest, runOnSunrise)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_idomTOOLS->runOnSunrise();
    std::string retStr = useful_F::myStaticData->myEventHandler.run("iDom")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("433MHz can not start due to home state: LOCK"));

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    auto ptr = static_cast<RADIO_SWITCH*>(test_my_data.main_REC->getEqPointer("B"));
    ptr->m_state = STATE::OFF;
    test_idomTOOLS->runOnSunrise();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::ON);

    RADIO_EQ_CONFIG cfg;
    cfg.sunset = "off";
    cfg.sunrise = "on";

    ptr->setCode(cfg);

    cfg.sunset = "on";
    cfg.sunrise = "off";

    ptr->setCode(cfg);
    ptr->m_state = STATE::ON;
    test_idomTOOLS->runOnSunrise();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, getSunrise_Sunset)
{
    std::string ret = test_idomTOOLS->getSunrise();
    EXPECT_THAT(ret, testing::HasSubstr(":"));
    std::cout << "sunrise: " << ret << std::endl;

    ret = test_idomTOOLS->getSunset();
    EXPECT_THAT(ret, testing::HasSubstr(":"));

    std::cout << "sunset: " << ret << std::endl;

    ret = test_idomTOOLS->getSunrise(true);
    EXPECT_THAT(ret, testing::HasSubstr("Sunrise time:"));

    ret = test_idomTOOLS->getSunset(true);
    EXPECT_THAT(ret, testing::HasSubstr("Sunset time:"));
}

TEST_F(iDomTOOLS_ClassTest, getDayLenght)
{
    std::string ret = test_idomTOOLS->getDayLenght();
    EXPECT_THAT(ret, testing::HasSubstr(":"));

    ret = test_idomTOOLS->getDayLenght(true);
    EXPECT_THAT(ret, testing::HasSubstr("Day Lenght :"));
}

TEST_F(iDomTOOLS_ClassTest, getTextToSpeach)
{
    TEST_DATA::return_send_to_arduino = "22:23";
    std::string ret = test_idomTOOLS->getTextToSpeach();
    EXPECT_THAT(ret, testing::HasSubstr("Smog:"));
    std::cout << "TEXT :"<< std::endl << ret << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, mpd)
{
    MPD_info test_ptr_MPD;
    test_ptr_MPD.volume = 3;
    test_my_data.ptr_MPD_info = &test_ptr_MPD;
    blockQueue test_q;
    test_q._clearAll();
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_idomTOOLS->MPD_play(&test_my_data);
    std::string retStr = useful_F::myStaticData->myEventHandler.run("MPD")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("MPD can not start due to home state: LOCK"));
    EXPECT_EQ(test_q._size(), 0);

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_idomTOOLS->MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 0);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;

    useful_F::myStaticData->myEventHandler.run("MPD")->clearEvent();
    test_idomTOOLS->MPD_play(&test_my_data,2);
    retStr = useful_F::myStaticData->myEventHandler.run("MPD")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("MPD can not start due to home state: LOCK"));
    EXPECT_EQ(test_q._size(), 0);

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_idomTOOLS->MPD_play(&test_my_data,2);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY_ID);
    EXPECT_EQ(test_q._size(), 0);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    test_idomTOOLS->MPD_stop();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(), 0);

    test_idomTOOLS->MPD_next();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::NEXT);
    EXPECT_EQ(test_q._size(), 0);

    test_idomTOOLS->MPD_prev();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PREV);
    EXPECT_EQ(test_q._size(), 0);

    test_idomTOOLS->MPD_pause();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PAUSE );
    EXPECT_EQ(test_q._size(), 0);

    test_idomTOOLS->MPD_volumeUp();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLUP );
    EXPECT_EQ(test_q._size(), 0);

    test_idomTOOLS->MPD_volumeDown();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLDOWN );
    EXPECT_EQ(test_q._size(), 0);

    test_idomTOOLS->MPD_volumeSet(&test_my_data, 99);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLSET );
    EXPECT_EQ(test_q._size(), 0);
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, 99);

    EXPECT_EQ(test_idomTOOLS->MPD_getVolume(&test_my_data),99);

}

TEST_F(iDomTOOLS_ClassTest, getTemperatureString)
{
    TEST_DATA::return_send_to_arduino = "-2:2";
    EXPECT_STREQ(test_my_data.main_iDomTools->getTemperatureString().c_str(), "-2:2");
}

TEST_F(iDomTOOLS_ClassTest, cameraLED)
{
    TEST_DATA::return_httpPost = "ok.\n";
    test_my_data.main_iDomTools->cameraLedOFF("test_link");

    ///////////////////////////////at day
    Clock::setTime_forBT_usage(12,12);
    test_my_data.main_iDomTools->cameraLedON("test_link");

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("cameraLED"), STATE::OFF);
    ////////////////////////////////////// at night
    Clock::setTime_forBT_usage(2,2);
    test_my_data.main_iDomTools->cameraLedON("test_link");

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("cameraLED"), STATE::ON);
}

TEST_F(iDomTOOLS_ClassTest, textToSpeach)
{
    test_my_data.ptr_MPD_info->isPlay = true;
    test_my_data.main_iDomStatus->setObjectState("speakers", STATE::UNDEFINE);

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);
    std::vector<std::string> test_v;
    test_my_data.main_iDomTools->textToSpeach(&test_v); //empty
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);

    test_v = {"test","msg","clock"};
    test_my_data.ptr_MPD_info->isPlay = true;
    test_my_data.main_iDomTools->textToSpeach(&test_v);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);
    test_my_data.ptr_MPD_info->isPlay = false;
    test_my_data.main_iDomTools->textToSpeach(&test_v);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, getWeatherEvent)
{
    TEST_DATA::return_httpPost = "httpPost";
    std::string retStr = test_my_data.main_iDomTools->getWeatherEvent("test",10);
    EXPECT_STREQ(retStr.c_str(),"httpPost");
}

TEST_F(iDomTOOLS_ClassTest, isItDay)
{
    Clock::setTime_forBT_usage(12,12);
    EXPECT_TRUE(test_my_data.main_iDomTools->isItDay());
    Clock::setTime_forBT_usage(2,12);
    EXPECT_FALSE(test_my_data.main_iDomTools->isItDay());
}

TEST_F(iDomTOOLS_ClassTest, ledClear)
{
    TEST_DATA::return_send_to_arduino = "done";
    std::string retStr = test_my_data.main_iDomTools->ledClear();
    EXPECT_STREQ(retStr.c_str(),"done");
}

TEST_F(iDomTOOLS_ClassTest, getAllDataSunrisesunset)
{
    EXPECT_THAT(test_my_data.main_iDomTools->getAllDataSunrisesunset(),
                testing::HasSubstr("Days until Y2K"));
}

TEST_F(iDomTOOLS_ClassTest, health_check)
{
    MQTT_mosquitto mainMQTT("iDomSERVER test");
    test_my_data.mqttHandler = &mainMQTT;
    test_my_data.mqttHandler->_subscribed = true;

    auto test_RFLink = new RFLinkHandler(&test_my_data);
    test_my_data.main_RFLink = test_RFLink;
    test_my_data.main_RFLink->m_pingTime = Clock::getUnixTime() - 321;
    test_my_data.main_iDomTools->healthCheck();
    EXPECT_EQ(test_my_data.iDomProgramState, iDomStateEnum::WORKING);
    test_my_data.main_RFLink->m_pingTime = Clock::getUnixTime() - 321;
    EXPECT_THROW(test_my_data.main_iDomTools->healthCheck(), std::string);
    EXPECT_EQ(test_my_data.iDomProgramState, iDomStateEnum::HARD_RELOAD);
    delete test_RFLink;
}

TEST_F(iDomTOOLS_ClassTest, access_KEYGEN)
{
    test_idomTOOLS->m_keyHandler->addKEY("test",256);
    std::cout << "KEY " << test_idomTOOLS->m_keyHandler->listKEY() << std::endl;
    test_idomTOOLS->m_keyHandler->addTempKEY("tmp",20);

    std::cout << "KEY " << test_idomTOOLS->m_keyHandler->listKEY() << std::endl;
    auto ret = test_idomTOOLS->m_keyHandler->getKEY("tmp");

    EXPECT_TRUE(test_idomTOOLS->m_keyHandler->useKEY("tmp",ret));

    std::cout << "KEY " << test_idomTOOLS->m_keyHandler->listKEY() << std::endl;

    ////// remove expired keys
    std::cout << std::endl << "======== remove expired keys ==========" << std::endl;
    test_idomTOOLS->m_keyHandler->addTempKEY("tmp1",20);
    test_idomTOOLS->m_keyHandler->addTempKEY("tmp2",20);

    std::cout << "KEY " << test_idomTOOLS->m_keyHandler->listKEY() << std::endl;
    sleep(1);
    ret = test_idomTOOLS->m_keyHandler->getKEY("tmp2");
    test_idomTOOLS->m_keyHandler->removeExpiredKeys(0);

    EXPECT_FALSE(test_idomTOOLS->m_keyHandler->useKEY("tmp2",ret));
}

TEST_F(iDomTOOLS_ClassTest, openGate_getLink)
{
    test_my_data.server_settings->_gateway.url = "http://tst.pl";
    test_my_data.server_settings->_gateway.keySize  = 128;
    auto ret = test_idomTOOLS->openGateLink();
    EXPECT_THAT(ret, testing::HasSubstr("http://"));

    std::string t_name = ret.substr(19,20);
    std::string t_key = ret.substr(44,128);

    EXPECT_TRUE(test_idomTOOLS->m_keyHandler->useKEY(t_name,t_key));

    std::cout << "wygenerowalem link: " << ret << std::endl;
}
