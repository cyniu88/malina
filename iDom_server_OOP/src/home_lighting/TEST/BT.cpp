#include <gtest/gtest.h>

#include "../house_lighting_handler.h"

class light_house_fixture : public testing::Test
{
protected:

    virtual void SetUp() final
    {
        std::cout << "konfiguracja przed testem light_house_fixture " <<std::endl;
    }

    virtual void TearDown() final
    {
        std::cout << "czyszczenie po tescie light_house_fixture" <<std::endl;
    }
};

TEST_F(light_house_fixture, main )
{
    house_lighting_handler lHandler;
    std::string nameRoom = "kuchnia";
    std::string nameBulb1 = "glowna";
    std::string nameBulb2 = "kiniet";

    lHandler.addRoom(nameRoom);
    lHandler.addBulbInRoom(nameRoom, nameBulb1,11);
    lHandler.addBulbInRoom(nameRoom, nameBulb2,12);

    lHandler.turnOnAllInRoom(nameRoom);

    EXPECT_EQ(STATE::ON, lHandler.m_lightingBulbMap[11]->getStatus());

    lHandler.m_lightingBulbMap[11]->off([](std::string name){puts("KOKO");});
    EXPECT_EQ(STATE::OFF, lHandler.m_lightingBulbMap[11]->getStatus());

}
