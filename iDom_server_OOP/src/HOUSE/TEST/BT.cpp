#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../house_room_handler.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

class house_fixture : public iDomTOOLS_ClassTest
{
public:
 std::unique_ptr<house_room_handler> testRoomHandler;
protected:

    virtual void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        std::string cfg("../config/bulb_config.json");
        testRoomHandler = std::make_unique<house_room_handler>(&test_my_data);
        testRoomHandler->loadConfig(cfg);

        test_my_data.mqttHandler = std::make_unique<MQTT_mosquitto>("iDomSERVER test");
        test_my_data.mqttHandler->_subscribed = true;
        useful_F::setStaticData(&test_my_data);
    }

    virtual void TearDown() final
    {
        std::cout << "czyszczenie po tescie light_house_fixture" <<std::endl;
        iDomTOOLS_ClassTest::TearDown();
    }
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
    RADIO_EQ_CONFIG tCfg;
    tCfg.name = "DingDong";
    tCfg.ID = "8899";
    test_rec->addRadioEq(tCfg, "SWITCH");
    std::string mqttMSG("state;88;-1;1;");
    testRoomHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap.at(88)->getStatus(), STATE::ON);
}

TEST_F(house_fixture, satelSensor)
{
    int bulbID = 127;
    int satelSensorID = 33;

    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::UNDEFINE);

    testRoomHandler->turnOffBulb(bulbID);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::DEACTIVE);

    testRoomHandler->satelSensorActive(satelSensorID);
    EXPECT_EQ(testRoomHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::ACTIVE);
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
