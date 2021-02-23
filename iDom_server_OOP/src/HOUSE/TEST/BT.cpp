#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../house_room_handler.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

class house_fixture : public iDomTOOLS_ClassTest
{
public:

protected:

    virtual void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        std::cout << "konfiguracja przed testem light_house_fixture " <<std::endl;
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

TEST_F(house_fixture, load_config)
{
    std::string cfg("../config/bulb_config.json");
    auto testRoomHandler = std::make_unique<house_room_handler>(&test_my_data);
    testRoomHandler->loadConfig(cfg);

    std::string ret = testRoomHandler->getAllInfoJSON().dump(4);

    EXPECT_THAT(ret, testing::HasSubstr(R"(room": "lazienka)"));
}

TEST_F(house_fixture, all_on_all_off_in_room)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_room_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    std::string ret = testLightingHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "UNDEFINE")"));

    testLightingHandler->turnOnAllInRoom("lazienka");

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[126]->getStatus() , STATE::ON);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[127]->getStatus() , STATE::ON);
    ret = testLightingHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "ON")"));

    testLightingHandler->turnOffAllInRoom("lazienka");

    ret = testLightingHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "OFF")"));
}


TEST_F(house_fixture, bulb_on_bulb_off)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_room_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[226]->getStatus() , STATE::UNDEFINE);

    testLightingHandler->turnOnBulb(226);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[226]->getStatus() , STATE::ON);

    testLightingHandler->turnOffBulb(226);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[226]->getStatus() , STATE::OFF);
}

TEST_F(house_fixture, bulb_status_from_mqtt) {
    test_my_data.main_iDomTools->lockHome();
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_room_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::UNDEFINE);

    std::string mqttMSG("state;226;-1;0;");
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::ON);

    mqttMSG = "state;226;44;0;";
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::OFF);

    mqttMSG = "state;226;44;1;";
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::ON);

    mqttMSG = "state;226;-1;1;";
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(226)->getStatus(), STATE::OFF);
}

TEST_F(house_fixture, dingDong)
{
    RADIO_EQ_CONFIG tCfg;
    tCfg.name = "DingDong";
    tCfg.ID = "8899";
    test_rec->addRadioEq(tCfg, "SWITCH");
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_room_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);
    std::string mqttMSG("state;888;-1;0;");
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(888)->getStatus(), STATE::ON);
}

TEST_F(house_fixture, satelSensor)
{
    int bulbID = 127;
    int satelSensorID = 33;
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_room_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::UNDEFINE);

    testLightingHandler->turnOffBulb(bulbID);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::OFF);

    testLightingHandler->satelSensorActive(satelSensorID);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[bulbID]->getStatus() , STATE::ON);
}

TEST_F(house_fixture, dump)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_room_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);
    std::cout << iDom_API::getDump() << std::endl;
}
