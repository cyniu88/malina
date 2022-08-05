#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../house_room_handler.h"
#include "../../iDom_server_OOP.h"
#include "../../functions/functions.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../iDomTools/mock/iDomToolsMock.h"

class house_fixture : public testing::Test
{
public:
    house_fixture():testRoomHandler(std::make_unique<house_room_handler>(&test_my_data)),
        test_rec(std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data)),
        main_iDomTools(std::make_shared<iDomToolsMock>())

    {
        std::string cfg("../config/bulb_config.json");
        testRoomHandler->loadConfig(cfg);

        test_my_data.mqttHandler = std::make_unique<MQTT_mosquitto>("iDomSERVER test");
        test_my_data.mqttHandler->_subscribed = true;
        useful_F::setStaticData(&test_my_data);
        test_my_data.main_iDomTools  = main_iDomTools;
        test_my_data.main_REC = test_rec;
        test_my_data.server_settings = &test_server_settings;
        test_my_data.server_settings->_fb_viber.viberSender = "test sender";
        test_my_data.server_settings->_fb_viber.viberReceiver = {"R1","R2"};
        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
    }
    std::unique_ptr<house_room_handler> testRoomHandler;
    thread_data test_my_data;
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    CONFIG_JSON test_server_settings;

};

TEST_F(house_fixture, load_config_bulb)
{
    std::string ret = testRoomHandler->getAllInfoJSON().dump(4);

    EXPECT_THAT(ret, testing::HasSubstr(R"(room": "lazienka)"));
}

TEST_F(house_fixture, all_on_all_off_in_room)
{
    std::string ret = testRoomHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "UNDEFINE")"));

    testRoomHandler->turnOnAllInRoom("pokoj dzieci");

    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[226]->getStatus() , STATE::ACTIVE);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[227]->getStatus() , STATE::ACTIVE);
    ret = testRoomHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "ACTIVE")"));

    testRoomHandler->turnOffAllInRoom("pokoj dzieci");

    ret = testRoomHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "DEACTIVE")"));
}


TEST_F(house_fixture, bulb_on_bulb_off)
{
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[226]->getStatus() , STATE::UNDEFINE);

    testRoomHandler->turnOnBulb(226);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[226]->getStatus() , STATE::ACTIVE);

    testRoomHandler->turnOffBulb(226);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[226]->getStatus() , STATE::DEACTIVE);
}

TEST_F(house_fixture, bulb_status_from_mqtt) {

    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::UNDEFINE);

    EXPECT_CALL(*main_iDomTools.get(), sendViberMsg("zmiana statusu lampy jedna w pomieszczeniu: pokoj dzieci na ON przyciskiem: -1 czas trwania: 00:00",
                                                    "R1",
                                                    "test sender-light",
                                                    "NULL",
                                                    "NULL"));
    std::string mqttMSG("state;226;-1;1;");
    testRoomHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::ON);

    mqttMSG = "state;226;44;0;";
    testRoomHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::OFF);

    mqttMSG = "state;226;44;1;";
    testRoomHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::ON);

    mqttMSG = "state;226;-1;0;";
    testRoomHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::OFF);
}

TEST_F(house_fixture, dingDong)
{
    EXPECT_CALL(*main_iDomTools.get(), sendViberPicture(testing::_,testing::_,testing::_,testing::_,testing::_,testing::_));
    EXPECT_CALL(*main_iDomTools.get(), sendViberMsg("zmiana statusu lampy bulbName w pomieszczeniu: roomName na ON przyciskiem: -1 czas trwania: 00:00",
                                                    "R1",
                                                    "test sender-light",
                                                    "NULL",
                                                    "NULL"));
    EXPECT_CALL(*main_iDomTools.get(), doorbellDingDong());
    std::string mqttMSG("state;88;-1;1;");
    testRoomHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(88)->getStatus(), STATE::ON);
}

TEST_F(house_fixture, satelSensor)
{
    int bulbID = 127;
    int satelSensorID = 33;
    EXPECT_CALL(*main_iDomTools.get(), isItDay());
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::UNDEFINE);

    testRoomHandler->turnOffBulb(bulbID);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::DEACTIVE);

    testRoomHandler->satelSensorActive(satelSensorID);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::ACTIVE);
}

TEST_F(house_fixture, satelSensorDouble)
{
    int bulbID = 327;
    int satelSensorID = 37;
    EXPECT_CALL(*main_iDomTools.get(), isItDay()).Times(2);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::UNDEFINE);

    testRoomHandler->turnOffBulb(bulbID);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::DEACTIVE);

    testRoomHandler->satelSensorActive(satelSensorID);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::ACTIVE);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[326]->getStatus() , STATE::ACTIVE);
}

TEST_F(house_fixture, dump)
{
    std::cout << iDom_API::getDump() << std::endl;
}

TEST_F(house_fixture, getStats)
{
    for(auto&a : testRoomHandler->m_roomMap){
        std::cout << a.second->getStatsJSON().dump(4) << std::endl;
    }
}

TEST_F(house_fixture, load_config_button)
{
    std::string cfg("../config/button_config.json");
    auto testRoomHandler = std::make_unique<house_room_handler>(&test_my_data);
    testRoomHandler->loadButtonConfig(cfg);
    CommandHandlerMQTT testCmdHandler;
    testRoomHandler->executeButtonComand(2, "long", &testCmdHandler);
}

TEST_F(house_fixture, satelSensorActive)
{
    testRoomHandler->turnOffAllInRoom("lazienka");
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(127)->getStatus(), STATE::DEACTIVE);

    test_my_data.idom_all_state.houseState = STATE::LOCK;
    EXPECT_CALL(*main_iDomTools.get(), isItDay()).WillOnce(testing::Return(false));
    EXPECT_CALL(*main_iDomTools.get(), sendViberMsg("alarm w pokoju lazienka",
                                                    "R1",
                                                    "Satel Alarm",
                                                    "NULL",
                                                    "NULL"));
    testRoomHandler->m_roomMap.at("lazienka")->satelSensorActive();
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(127)->getStatus(), STATE::ACTIVE);
}
