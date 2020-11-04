#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../house_lighting_handler.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

class light_house_fixture : public iDomTOOLS_ClassTest
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

TEST_F(light_house_fixture, load_config)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    std::string ret = testLightingHandler->getAllInfoJSON().dump(4);

    EXPECT_THAT(ret, testing::HasSubstr(R"(room": "lazienka)"));
}

TEST_F(light_house_fixture, all_on_all_off)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    std::string ret = testLightingHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "UNDEFINE")"));

    testLightingHandler->turnOnAllInRoom("lazienka");

    ret = testLightingHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "ON")"));

    testLightingHandler->turnOffAllInRoom("lazienka");

    ret = testLightingHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr(R"(STATUS": "OFF")"));
}


TEST_F(light_house_fixture, bulb_on_bulb_off)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[111]->getStatus() , STATE::UNDEFINE);

    testLightingHandler->turnOnBulb(111);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[111]->getStatus() , STATE::ON);

    testLightingHandler->turnOffBulb(111);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[111]->getStatus() , STATE::OFF);
}

TEST_F(light_house_fixture, bulb_status_from_mqtt) {
    test_my_data.main_iDomTools->lockHome();
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(111)->getStatus(), STATE::UNDEFINE);

    std::string mqttMSG("state;111;-1;0;");
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(111)->getStatus(), STATE::ON);

    mqttMSG = "state;111;44;0;";
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(111)->getStatus(), STATE::OFF);

    mqttMSG = "state;111;44;1;";
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(111)->getStatus(), STATE::ON);

    mqttMSG = "state;111;-1;1;";
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(111)->getStatus(), STATE::OFF);
}

TEST_F(light_house_fixture, dingDong)
{
    RADIO_EQ_CONFIG tCfg;
    tCfg.name = "DingDong";
    tCfg.ID = "8899";
    test_rec->addRadioEq(tCfg, "SWITCH");
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);
    std::string mqttMSG("state;888;-1;0;");
    testLightingHandler->executeCommandFromMQTT(mqttMSG);
    EXPECT_EQ(testLightingHandler->m_lightingBulbMap.at(888)->getStatus(), STATE::ON);
}

TEST_F(light_house_fixture, dump)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(&test_my_data);
    testLightingHandler->loadConfig(cfg);
    std::cout << iDom_API::getDump() << std::endl;
}
