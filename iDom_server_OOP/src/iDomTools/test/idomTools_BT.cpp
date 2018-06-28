#include <gtest/gtest.h>
#include <curl/curl.h>

#include "iDomTools_fixture.h"


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
    return"{\"message_status\":\"ok\"}";
}
std::string viber_API::sendViberPicture(std::string msg,
                                        std::string image,
                                        std::string receiver,
                                        std::string senderName,
                                        std::string accessToken ,
                                        std::string url){
    std::cout << "sendViberPicture() "<< msg <<" to: "<< receiver << std::endl;
    TEST_DATA::return_viber_msg = msg;
    return"{\"message_status\":\"ok\"}";
}
FACEBOOK_API::FACEBOOK_API(){}
std::string FACEBOOK_API::postTxtOnWall(std::string msg, std::string accessToken ){return "";}
std::string FACEBOOK_API::postPhotoOnWall(std::string url,std::string msg ,std::string accessToken ){return "";}
void FACEBOOK_API::setAccessToken(std::string token){}

void LCD_c::set_lcd_STATE(int i){}
void LCD_c::printString(bool clear, int col, int row, std::string str){
    std::cout << "LCD_c::printString() "<< str  << std::endl;
}

std::string useful_F_libs::httpPost(std::string url, int timeoutSeconds){

    std::cout << "url: "<< url << std::endl;
    TEST_DATA::return_httpPost_expect = "httpPost";
    return TEST_DATA::return_httpPost;
}
std::string useful_F_libs::httpPost(std::string url){
CURL *curl;
CURLcode res;
std::string readBuffer;
curl = curl_easy_init();

if(curl) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, useful_F_libs::WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
}
curl_global_cleanup();

return readBuffer;
}

TEST_F(iDomTOOLS_ClassTest, smog)
{
    std::string smog = test_idomTOOLS->getSmog();
    puts(smog.c_str());
    puts(" smog");
    ASSERT_GE(smog.size(),2);

    unsigned int smog_int = std::stoi(smog);
    ASSERT_GT(smog_int,1);
    ASSERT_LT(smog_int,1000);
}

TEST_F(iDomTOOLS_ClassTest, hasTemperatureChange)
{
    std::cout << "##################################### 0" <<std::endl;

    TEST_DATA::return_send_to_arduino = "20.0:-1.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("outside"),TEMPERATURE_STATE::Under);
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);
    std::cout << "##################################### 1" <<std::endl;

    TEST_DATA::return_send_to_arduino = "25.4:0.0;";
    test_idomTOOLS->send_temperature_thingSpeak();

    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("inside"),TEMPERATURE_STATE::Over);

    std::cout << "##################################### 2" <<std::endl;

    TEST_DATA::return_send_to_arduino = "21.0:1.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("outside"),TEMPERATURE_STATE::Over);
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("inside"),TEMPERATURE_STATE::Under);

    std::cout << "##################################### 3" <<std::endl;
    TEST_DATA::return_send_to_arduino = "21.0:5.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);

    std::cout << "##################################### 4" <<std::endl;
    TEST_DATA::return_send_to_arduino = "21.0:4.0;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("inside"),TEMPERATURE_STATE::NoChanges);
    std::cout << "##################################### 5" <<std::endl;

    TEST_DATA::return_send_to_arduino = "31.9:11.11;";
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("outside"),TEMPERATURE_STATE::NoChanges);
    EXPECT_EQ(test_idomTOOLS->allThermometer.getLastState("inside"),TEMPERATURE_STATE::Over);
    std::cout << "##################################### 6" <<std::endl;
}

TEST_F(iDomTOOLS_ClassTest, weatherAlert)
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


    std::vector<WEATHER_ALER> test_WA;
    test_WA =  test_idomTOOLS->getAlert(test_data_from_www);
    EXPECT_EQ(1,test_WA.size()) << "ZŁY ROZMIAR VEKTORA WA";
}

TEST_F(iDomTOOLS_ClassTest, send_temperature_thingSpeak){

    TEST_DATA::return_httpPost_expect = "NULL";

    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"NULL");
    test_idomTOOLS->send_temperature_thingSpeak();
    EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(),"httpPost");
}

TEST_F(iDomTOOLS_ClassTest, checkAlarm)
{
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
    test_status.setObjectState("alarm", test_alarmTime.state);
    test_my_data.alarmTime = test_alarmTime;
    useful_F::myStaticData = &test_my_data;

    //////////////////////////////////////////////////////////////

    MPD_info test_ptr_MPD;
    test_ptr_MPD.volume = 3;
    test_my_data.ptr_MPD_info = &test_ptr_MPD;
    RADIO_EQ_CONFIG cfg;
    cfg.name = "listwa";
    cfg.ID = "209888";
    RADIO_EQ* test_RS = new RADIO_SWITCH(&test_my_data,cfg,RADIO_EQ_TYPE::SWITCH);

    RADIO_EQ_CONTAINER_STUB stub_rec(&test_my_data);
    test_my_data.main_REC = (&stub_rec);
    test_my_data.alarmTime.time = Clock::getTime();
    test_my_data.alarmTime.state = STATE::ACTIVE;

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::ACTIVE);

    for(unsigned int i = fromVol; i<toVol; ++i)
    {
        test_idomTOOLS->checkAlarm();

        EXPECT_EQ(test_my_data.alarmTime.state, STATE::WORKING)<< "zły stan w for " << i<< " "<< toVol;
        EXPECT_EQ(test_my_data.ptr_MPD_info->volume, i+1) << "zły poziom glosnosci w for";
    }
    EXPECT_CALL(stub_rec, getEqPointer("ALARM")).WillRepeatedly(testing::Return(test_RS));
    test_idomTOOLS->checkAlarm();

    EXPECT_EQ(test_my_data.alarmTime.state, STATE::DEACTIVE) << "nie jest STATE::DEACTIVE";
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, toVol)<< "nie inkrementowane?";

    delete test_RS;
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
}

TEST_F(iDomTOOLS_ClassTest, buttonPressed)
{
    std::string button433MHz_id = "01e7be";
    std::string pressedButtonName = test_idomTOOLS->buttonPressed(button433MHz_id);
    EXPECT_EQ(1, test_my_data.main_REC->getButtonPointerVector().size());
    EXPECT_STREQ(std::to_string(button433MHz_id).c_str(),
                 test_my_data.main_REC->getButtonPointerVector().at(0)->getID().c_str());
    EXPECT_STREQ(pressedButtonName.c_str(), "locker");

    EXPECT_THROW(test_idomTOOLS->buttonPressed(button433MHz_id+"a"),
                 std::string);
}

TEST_F(iDomTOOLS_ClassTest, button433MHzPressedAction_lockerUnlock)
{
    blockQueue test_q;
    test_q._clearAll();

    test_idomTOOLS->unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);

    for(auto i : {1,2,3}){
        test_idomTOOLS->button433MHzPressedAction("locker");
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

    test_idomTOOLS->button433MHzPressedAction("locker");
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK)<< "nie jest UNLOCK";

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
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

TEST_F(iDomTOOLS_ClassTest, saveState)
{



    test_status.setObjectState("house",STATE::UNLOCK);
//////////////////// mpd
    test_status.setObjectState("music", STATE::PLAY);
    test_status.setObjectState("speakers", STATE::ON);
    test_my_data.idom_all_state.houseState = STATE::LOCK;

    test_status.setObjectState("listwa",STATE::ON);
    test_alarmTime.time = Clock::getTime();
    test_alarmTime.state = STATE::ACTIVE;
    test_status.setObjectState("alarm", test_alarmTime.state);

    test_idomTOOLS->saveState_iDom();

    nlohmann::json testJson;
    std::ifstream i(test_server_set.saveFilePath);
    i >> testJson;
    EXPECT_STREQ(test_status.getObjectStateString("music").c_str(),
                 testJson.at("MPD").at("music").get<std::string>().c_str() );
    EXPECT_STREQ((test_status.getObjectStateString("alarm")).c_str(),
                 testJson.at("ALARM").at("alarm").get<std::string>().c_str() );
}

TEST_F(iDomTOOLS_ClassTest, readState)
{
    test_idomTOOLS->readState_iDom();
    EXPECT_EQ(test_my_data.alarmTime.state,STATE::ACTIVE);
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
    test_my_data.server_settings->lightningApiURL = "http://cyniu88.no-ip.pl/test/json/lightning.json";
    test_idomTOOLS->checkLightning();
    auto test_alert_info = test_idomTOOLS->getLightningStruct();
    EXPECT_EQ(test_alert_info.timestamp,210);
}
