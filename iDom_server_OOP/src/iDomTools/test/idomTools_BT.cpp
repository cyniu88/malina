#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <curl/curl.h>

#include "iDomTools_fixture.h"
#include "../mqtt_mosquitto/MQTT_mosquitto/mqtt.h"
#include "../../SATEL_INTEGRA/BT/satel_integra_stub.h"

TEST_F(iDomTOOLS_ClassTest, smog)
{
    std::string smog = test_my_data.main_iDomTools->getSmog();
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

    test_my_data.lusina.statHumi.push_back(12);
    test_my_data.lusina.statTemp.push_back(1.133333);
    test_my_data.lusina.statTemp.push_back(1.443553);
    test_my_data.lusina.statTemp.push_back(1.34435);

    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(-7.7));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-1.0));
    EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
    test_my_data.main_iDomTools->send_data_to_thingSpeak();

    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"),TEMPERATURE_STATE::Under);
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"),TEMPERATURE_STATE::NoChanges);
    std::cout << "##################################### 1" <<std::endl;

    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(-7.7));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(27.7));
    EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
    test_my_data.main_iDomTools->send_data_to_thingSpeak();

    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"),TEMPERATURE_STATE::Over);

    std::cout << "##################################### 2" <<std::endl;

    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(20.3));
    EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
    test_my_data.main_iDomTools->send_data_to_thingSpeak();
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"),TEMPERATURE_STATE::Over);
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"),TEMPERATURE_STATE::Under);

    std::cout << "##################################### 3" <<std::endl;

    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-0.3));
    EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
    test_my_data.main_iDomTools->send_data_to_thingSpeak();
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"),TEMPERATURE_STATE::NoChanges);

    std::cout << "##################################### 4" <<std::endl;

    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-0.3));
    EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
    test_my_data.main_iDomTools->send_data_to_thingSpeak();
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"),TEMPERATURE_STATE::NoChanges);

    std::cout << "##################################### 5" <<std::endl;

    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(30.3));
    EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
    test_my_data.main_iDomTools->send_data_to_thingSpeak();
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_my_data.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"),TEMPERATURE_STATE::Over);
    std::cout << "##################################### 6" <<std::endl;

    ////// getThermoStats

    std::cout << "WYNIK: " << test_my_data.main_iDomTools->getThermoStats("inside") <<std::endl;
}

TEST_F(iDomTOOLS_ClassTest, weatherAlert)
{
   /* std::string test_data_from_www = R"( <div style="margin:0;padding:0;width:350px;font:0.8em Lucida,Arial,sans-seri f;background:#FFC">)"
                                     R"(<p style="margin:1px;padding:1px;text-align:center;background:#FF9;borde \\ r:1px dotted"><b><a href="http://burze.dzis.net?page=wyszukiwarka&amp;miejscowos\\ c=krakow" target="_blank" style="color:#00E">krakow</a></b>)"
                                     R"(<i>(50°03'N 19°57'E)</i> </p>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;cl ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:\\ center">Zarejestrowano 54 wyładowania atmosferyczne w promieniu 300km . Najbliższe 79.76km na zachód.)"
                                     R"(</dl> <dl style="margin:1px 1px 0 1px;padding:0;cl ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align: \\ center">Mróz, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Upał, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Wiatr, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Opady, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Burze, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Trąby powietrzne, brak ostrzeżeń</dl></div>)";
*/
    WEATHER_DATABASE test_WA;
    test_WA = test_my_data.main_iDomTools->getAlert();
    std::cout << std::endl << test_WA.lightning.alert<< std::endl;
}

TEST_F(iDomTOOLS_ClassTest, send_data_to_thingSpeak){

    TEST_DATA::return_send_to_arduino = "-2.3:-2";
    TEST_DATA::return_httpPost_expect = "NULL";
    test_my_data.lusina.statHumi.push_back(12);
    test_my_data.lusina.statTemp.push_back(1.13);
    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"NULL");
    test_my_data.main_iDomTools->send_data_to_thingSpeak();
    std::cout << "DATA: "<< TEST_DATA::return_httpPost_expect <<std::endl;
    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"httpPost");
}

TEST_F(iDomTOOLS_ClassTest, checkAlarm)
{
    blockQueue test_q;
    unsigned int fromVol = 48;
    unsigned int  toVol = 57;

    ///////////////////////////////////// to save
    test_my_data.main_iDomStatus->setObjectState("house",STATE::UNLOCK);
    test_my_data.main_iDomStatus->setObjectState("music", STATE::PLAY);
    test_my_data.main_iDomStatus->setObjectState("speakers", STATE::ON);
    test_my_data.idom_all_state.houseState = STATE::LOCK;

    test_my_data.main_iDomStatus->setObjectState("listwa",STATE::ON);

    test_alarmTime.time = Clock::getTime();
    test_alarmTime.state = STATE::ACTIVE;
    test_alarmTime.toVolume = 58;
    test_alarmTime.fromVolume = 48;
    test_my_data.main_iDomStatus->setObjectState("alarm", test_alarmTime.state);
    test_my_data.alarmTime = test_alarmTime;
    useful_F::myStaticData = &test_my_data;

    //////////////////////////////////////////////////////////////

    test_my_data.alarmTime.time = Clock::getTime();
    test_my_data.alarmTime.state = STATE::ACTIVE;

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::ACTIVE);

    for(unsigned int i = fromVol; i<toVol; ++i)
    {
        test_my_data.main_iDomTools->checkAlarm();
        test_q._get();
        EXPECT_EQ(test_my_data.alarmTime.state, STATE::WORKING)<< "zły stan w for " << i<< " "<< toVol;
        EXPECT_EQ(test_my_data.ptr_MPD_info->volume, i + 1) << "zły poziom glosnosci w for";
    }

    test_my_data.main_iDomTools->checkAlarm();

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::DEACTIVE) << "nie jest STATE::DEACTIVE";
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, toVol)<< "nie inkrementowane?";

}

TEST_F(iDomTOOLS_ClassTest, homeLockPlayStopMusic)
{
    ///////////////////////////////////// to save
    test_my_data.main_iDomStatus->setObjectState("house",STATE::UNDEFINE);
    test_my_data.main_iDomStatus->setObjectState("music", STATE::PLAY);
    test_my_data.main_iDomStatus->setObjectState("speakers", STATE::ON);
    test_my_data.idom_all_state.houseState = STATE::UNDEFINE;

    test_my_data.main_iDomStatus->setObjectState("listwa",STATE::ON);

    test_alarmTime.time = Clock::getTime();
    test_alarmTime.state = STATE::ACTIVE;
    test_my_data.main_iDomStatus->setObjectState("alarm", test_alarmTime.state);

    blockQueue test_q;
    test_q._clearAll();
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNDEFINE);
    test_my_data.main_iDomTools->lockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::LOCK);
    test_my_data.main_iDomTools->MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(),0);
    test_my_data.main_iDomTools->unlockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNLOCK);
    test_my_data.main_iDomTools->MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
    test_my_data.main_iDomTools->lockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::LOCK);
    test_my_data.main_iDomTools->MPD_stop();
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::LOCK);
    std::string returnedString = test_my_data.main_iDomStatus->getAllObjectsStateString();
    EXPECT_THAT(returnedString, testing::HasSubstr("LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, testCPU_Load)
{
    std::cout <<"TEST LOAD" << std::endl;
    std::cout << test_my_data.main_iDomTools->getSystemInfo() << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, saveState_readState)
{
    test_my_data.main_iDomStatus->setObjectState("house",STATE::UNLOCK);
    //////////////////// mpd
    test_my_data.main_iDomStatus->setObjectState("music", STATE::PLAY);
    test_my_data.main_iDomStatus->setObjectState("speakers", STATE::ON);
    test_my_data.idom_all_state.houseState = STATE::LOCK;

    test_my_data.main_iDomStatus->setObjectState("listwa",STATE::ON);
    test_alarmTime.time = Clock::getTime();
    test_alarmTime.state = STATE::ACTIVE;
    test_alarmTime.fromVolume = 0;
    test_alarmTime.toVolume = 100;
    test_alarmTime.radioID = 44;
    test_my_data.alarmTime = test_alarmTime;
    test_my_data.main_iDomStatus->setObjectState("alarm", test_alarmTime.state);
    test_my_data.serverStarted = true;

    test_my_data.main_iDomTools->saveState_iDom(test_my_data.serverStarted);

    nlohmann::json testJson;
    std::ifstream i(test_server_set._server.saveFilePath);
    i >> testJson;
    EXPECT_STREQ(test_my_data.main_iDomStatus->getObjectStateString("music").c_str(),
                 testJson.at("MPD").at("music").get<std::string>().c_str() );
    EXPECT_STREQ((test_my_data.main_iDomStatus->getObjectStateString("alarm")).c_str(),
                 testJson.at("ALARM").at("alarm").get<std::string>().c_str() );
    EXPECT_EQ(test_alarmTime.radioID,
              testJson.at("ALARM").at("radioID").get<int>() );
    EXPECT_EQ(test_alarmTime.fromVolume,
              testJson.at("ALARM").at("fromVolume").get<int>() );
    EXPECT_EQ(test_alarmTime.toVolume,
              testJson.at("ALARM").at("toVolume").get<int>() );

    ////////////////////////////////// read
    iDom_SAVE_STATE info(test_my_data.server_settings->_server.saveFilePath);
    test_my_data.main_iDomTools->readState_iDom(info.read());
    EXPECT_EQ(test_my_data.alarmTime.state,STATE::ACTIVE);

    test_my_data.server_settings->_server.saveFilePath = "null";
    EXPECT_NO_THROW(test_my_data.main_iDomTools->readState_iDom(info.read()));
}

TEST_F(iDomTOOLS_ClassTest, checkLightning)
{
    test_my_data.server_settings->_server.lightningApiURL = "http://45.90.3.84/test/json/lightning.json";
    test_my_data.main_iDomTools->checkLightning();
}

TEST_F(iDomTOOLS_ClassTest, updateTemperatureStats)
{
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));
    test_my_data.main_iDomTools->updateTemperatureStats();
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(16));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(16));

    test_my_data.main_iDomTools->updateTemperatureStats();

    ////////////// maleje na mieskzaniu
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(16));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));

    test_my_data.main_iDomTools->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura maleje"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("mieszkaniu"));

    ////////////// maleje na polu
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));

    test_my_data.main_iDomTools->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura maleje"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("polu"));

    ////////////// rośnie na mieskzaniu
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(17));

    test_my_data.main_iDomTools->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura rośnie"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("mieszkaniu"));

    ////////////// rośnie na polu
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(17));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(17));

    test_my_data.main_iDomTools->updateTemperatureStats();
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("temperatura rośnie"));
    EXPECT_THAT(TEST_DATA::return_viber_msg,testing::HasSubstr("polu"));
}

TEST_F(iDomTOOLS_ClassTest, speakersON_OFF)
{
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_my_data.main_iDomTools->turnOnSpeakers();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::ON);
    test_my_data.main_iDomTools->turnOffSpeakers();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_my_data.main_iDomTools->turnOnSpeakers();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
    std::string retStr = useful_F::myStaticData->myEventHandler.run("speakers")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("speakers can not start due to home state: LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, printerON_OFF)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_my_data.main_iDomTools->turnOnPrinter();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"), STATE::ON);
    test_my_data.main_iDomTools->turnOffPrinter();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"), STATE::OFF);
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_my_data.main_iDomTools->turnOnPrinter();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"), STATE::OFF);
    std::string retStr = useful_F::myStaticData->myEventHandler.run("230V")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("Printer can not start due to home state: LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, getPinState)
{
    setReturnPinState(0);
    EXPECT_EQ(test_my_data.main_iDomTools->getPinState(0), PIN_STATE::LOW_STATE);
    setReturnPinState(1);
    EXPECT_EQ(test_my_data.main_iDomTools->getPinState(0), PIN_STATE::HIGH_STATE);
    setReturnPinState(4);
    EXPECT_EQ(test_my_data.main_iDomTools->getPinState(0), PIN_STATE::UNKNOWN_STATE);
}

TEST_F(iDomTOOLS_ClassTest, onOff230vOutdoor)
{
 //   test_my_data.main_iDomTools->turnOnOff230vOutdoor();
}

TEST_F(iDomTOOLS_ClassTest, turn_On_Off_433MHzSwitch)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::UNKNOWN);
    test_my_data.main_iDomTools->turnOn433MHzSwitch("B");
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::ON);
    test_my_data.main_iDomTools->turnOff433MHzSwitch("B");
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, turn_On_Off_fake_433MHzSwitch)
{
    EXPECT_NO_THROW(test_my_data.main_iDomTools->turnOn433MHzSwitch("B-fake"));
    EXPECT_NO_THROW(test_my_data.main_iDomTools->turnOff433MHzSwitch("B-fake"));
    EXPECT_NO_THROW(test_my_data.main_iDomTools->turnOnOff433MHzSwitch("B-fake"));
}

TEST_F(iDomTOOLS_ClassTest, turnOnOff433MHzSwitch)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::UNKNOWN);
    test_my_data.main_iDomStatus->setObjectState("B",STATE::ON);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::ON);
    test_my_data.main_iDomTools->turnOnOff433MHzSwitch("B");
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::OFF);
    test_my_data.main_iDomTools->turnOnOff433MHzSwitch("B");
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("B"),STATE::ON);

}

TEST_F(iDomTOOLS_ClassTest, runOnSunset)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_my_data.main_iDomTools->runOnSunset();
    std::string retStr = useful_F::myStaticData->myEventHandler.run("iDom")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("433MHz can not start due to home state: LOCK"));

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    auto ptr = static_cast<RADIO_SWITCH*>(test_my_data.main_REC->getEqPointer("B"));
    ptr->m_state = STATE::ON;
    test_my_data.main_iDomTools->runOnSunset();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::OFF);

    RADIO_EQ_CONFIG cfg;
    cfg.sunset = "on";
    cfg.sunrise = "off";

    ptr->setCode(cfg);
    ptr->m_state = STATE::OFF;
    test_my_data.main_iDomTools->runOnSunset();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::ON);
}

TEST_F(iDomTOOLS_ClassTest, runOnSunrise)
{
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_my_data.main_iDomTools->runOnSunrise();
    std::string retStr = useful_F::myStaticData->myEventHandler.run("iDom")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("433MHz can not start due to home state: LOCK"));

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    auto ptr = static_cast<RADIO_SWITCH*>(test_my_data.main_REC->getEqPointer("B"));
    ptr->m_state = STATE::OFF;
    test_my_data.main_iDomTools->runOnSunrise();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::OFF);

    RADIO_EQ_CONFIG cfg;
    cfg.sunset = "off";
    cfg.sunrise = "on";

    ptr->setCode(cfg);

    cfg.sunset = "on";
    cfg.sunrise = "off";

    ptr->setCode(cfg);
    ptr->m_state = STATE::ON;
    test_my_data.main_iDomTools->runOnSunrise();

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, getSunrise_Sunset)
{
    std::string ret = test_my_data.main_iDomTools->getSunrise();
    EXPECT_THAT(ret, testing::HasSubstr(":"));
    std::cout << "sunrise: " << ret << std::endl;

    ret = test_my_data.main_iDomTools->getSunset();
    EXPECT_THAT(ret, testing::HasSubstr(":"));

    std::cout << "sunset: " << ret << std::endl;

    ret = test_my_data.main_iDomTools->getSunrise(true);
    EXPECT_THAT(ret, testing::HasSubstr("Sunrise time:"));

    ret = test_my_data.main_iDomTools->getSunset(true);
    EXPECT_THAT(ret, testing::HasSubstr("Sunset time:"));
}

TEST_F(iDomTOOLS_ClassTest, getDayLenght)
{
    std::string ret = test_my_data.main_iDomTools->getDayLenght();
    EXPECT_THAT(ret, testing::HasSubstr(":"));

    ret = test_my_data.main_iDomTools->getDayLenght(true);
    EXPECT_THAT(ret, testing::HasSubstr("Day Lenght :"));
}

TEST_F(iDomTOOLS_ClassTest, getTextToSpeach)
{
    TEST_DATA::return_send_to_arduino = "22:23";
    std::string ret = test_my_data.main_iDomTools->getTextToSpeach();
    EXPECT_THAT(ret, testing::HasSubstr("Smog:"));
    std::cout << "TEXT :"<< std::endl << ret << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, mpd)
{
    test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();
    test_my_data.ptr_MPD_info->volume = 3;
    blockQueue test_q;
    test_q._clearAll();
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;
    test_my_data.main_iDomTools->MPD_play(&test_my_data);
    std::string retStr = useful_F::myStaticData->myEventHandler.run("MPD")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("MPD can not start due to home state: LOCK"));
    EXPECT_EQ(test_q._size(), 0);

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_my_data.main_iDomTools->MPD_play(&test_my_data);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 0);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    useful_F::myStaticData->idom_all_state.houseState = STATE::LOCK;

    useful_F::myStaticData->myEventHandler.run("MPD")->clearEvent();
    test_my_data.main_iDomTools->MPD_play(&test_my_data,2);
    retStr = useful_F::myStaticData->myEventHandler.run("MPD")->getEvent();
    EXPECT_THAT(retStr, testing::HasSubstr("MPD can not start due to home state: LOCK"));
    EXPECT_EQ(test_q._size(), 0);

    useful_F::myStaticData->idom_all_state.houseState = STATE::UNLOCK;
    test_my_data.main_iDomTools->MPD_play(&test_my_data,2);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY_ID);
    EXPECT_EQ(test_q._size(), 0);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    test_my_data.main_iDomTools->MPD_stop();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(), 0);

    test_my_data.main_iDomTools->MPD_next();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::NEXT);
    EXPECT_EQ(test_q._size(), 0);

    test_my_data.main_iDomTools->MPD_prev();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PREV);
    EXPECT_EQ(test_q._size(), 0);

    test_my_data.main_iDomTools->MPD_pause();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PAUSE );
    EXPECT_EQ(test_q._size(), 0);

    test_my_data.main_iDomTools->MPD_volumeUp();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLUP );
    EXPECT_EQ(test_q._size(), 0);

    test_my_data.main_iDomTools->MPD_volumeDown();
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLDOWN );
    EXPECT_EQ(test_q._size(), 0);

    test_my_data.main_iDomTools->MPD_volumeSet(&test_my_data, 99);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLSET );
    EXPECT_EQ(test_q._size(), 0);
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, 99);

    EXPECT_EQ(test_my_data.main_iDomTools->MPD_getVolume(&test_my_data),99);

}

TEST_F(iDomTOOLS_ClassTest, getTemperatureString)
{
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-0.3));
    EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(62.2));

    EXPECT_STREQ(test_my_data.main_iDomTools->getTemperatureString().c_str(), "-0.3:1.1:62.2:");
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

TEST_F(iDomTOOLS_ClassTest, getAllDataSunriseSunset)
{
    EXPECT_THAT(test_my_data.main_iDomTools->getAllDataSunriseSunset(),
                testing::HasSubstr("Days until Y2K"));
}

TEST_F(iDomTOOLS_ClassTest, health_check)
{
    test_my_data.mqttHandler = std::make_unique<MQTT_mosquitto>("iDomSERVER test");
    test_my_data.mqttHandler->_subscribed = true;


    test_my_data.main_RFLink = std::make_shared<RFLinkHandler>(&test_my_data);
    test_my_data.main_RFLink->m_pingTime = Clock::getUnixTime() - 321;
    test_my_data.main_iDomTools->healthCheck();
    EXPECT_EQ(test_my_data.iDomProgramState, iDomStateEnum::WORKING);
    test_my_data.main_RFLink->m_pingTime = Clock::getUnixTime() - 321;
    EXPECT_THROW(test_my_data.main_iDomTools->healthCheck(), std::string);
    EXPECT_EQ(test_my_data.iDomProgramState, iDomStateEnum::HARD_RELOAD);
}
/*
TEST_F(iDomTOOLS_ClassTest, access_KEYGEN)
{
    test_my_data.main_iDomTools->m_keyHandler->addKEY("test",256);
    std::cout << "KEY " << test_my_data.main_iDomTools->m_keyHandler->listKEY() << std::endl;
    test_my_data.main_iDomTools->m_keyHandler->addTempKEY("tmp",20);

    std::cout << "KEY " << test_my_data.main_iDomTools->m_keyHandler->listKEY() << std::endl;
    auto ret = test_my_data.main_iDomTools->m_keyHandler->getKEY("tmp");

    EXPECT_TRUE(test_my_data.main_iDomTools->m_keyHandler->useKEY("tmp",ret));

    std::cout << "KEY " << test_my_data.main_iDomTools->m_keyHandler->listKEY() << std::endl;

    ////// remove expired keys
    std::cout << std::endl << "======== remove expired keys ==========" << std::endl;
    test_my_data.main_iDomTools->m_keyHandler->addTempKEY("tmp1",20);
    test_my_data.main_iDomTools->m_keyHandler->addTempKEY("tmp2",20);

    std::cout << "KEY " << test_my_data.main_iDomTools->m_keyHandler->listKEY() << std::endl;
    sleep(1);
    ret = test_my_data.main_iDomTools->m_keyHandler->getKEY("tmp2");
    test_my_data.main_iDomTools->m_keyHandler->removeExpiredKeys(0);

    EXPECT_FALSE(test_my_data.main_iDomTools->m_keyHandler->useKEY("tmp2",ret));
}

TEST_F(iDomTOOLS_ClassTest, openGate_getLink)
{
    test_my_data.server_settings->_gateway.url = "http://tst.pl?";
    test_my_data.server_settings->_gateway.keySize = 128;
    auto ret = test_my_data.main_iDomTools->openGateLink({"t1","t2"});
    EXPECT_THAT(ret, testing::HasSubstr("http://"));

    std::string t_name = ret.substr(14,20);
    std::string t_key = ret.substr(35,128);

    EXPECT_TRUE(test_my_data.main_iDomTools->m_keyHandler->useKEY(t_name,t_key));

    std::cout << "wygenerowalem link: " << ret << std::endl;
}
*/
