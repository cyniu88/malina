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
        std::cout << "konfiguracja przed testem light_house_fixture " <<std::endl;
        test_my_data.mqttHandler = std::make_unique<MQTT_mosquitto>("iDomSERVER test");
        test_my_data.mqttHandler->_subscribed = true;
        useful_F::setStaticData(&test_my_data);
    }

    virtual void TearDown() final
    {
        std::cout << "czyszczenie po tescie light_house_fixture" <<std::endl;
    }
};

TEST_F(light_house_fixture, load_config)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(cfg);

    std::string ret = testLightingHandler->getAllInfoJSON().dump(4);

    EXPECT_THAT(ret, testing::HasSubstr("room\": \"lazienka"));
}

TEST_F(light_house_fixture, all_on_all_off)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(cfg);

    std::string ret = testLightingHandler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr("STATUS\": \"UNDEFINE\""));

     testLightingHandler->turnOnAllInRoom("lazienka");

     ret = testLightingHandler->getAllInfoJSON().dump(4);
     EXPECT_THAT(ret, testing::HasSubstr("STATUS\": \"ON\""));

   testLightingHandler->turnOffAllInRoom("lazienka");

   ret = testLightingHandler->getAllInfoJSON().dump(4);
   EXPECT_THAT(ret, testing::HasSubstr("STATUS\": \"OFF\""));
}


TEST_F(light_house_fixture, bulb_on_bulb_off)
{
    std::string cfg("../config/bulb_config.json");
    auto testLightingHandler = std::make_unique<house_lighting_handler>(cfg);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[100]->getStatus() , STATE::UNDEFINE);

    testLightingHandler->turnOnBulb(100);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[100]->getStatus() , STATE::ON);

    testLightingHandler->turnOffBulb(100);

    EXPECT_EQ(testLightingHandler->m_lightingBulbMap[100]->getStatus() , STATE::OFF);
}

TEST_F(light_house_fixture, dump)
{
    std::cout << iDom_API::getDump() << std::endl;
}

