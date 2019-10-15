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

TEST_F(light_house_fixture, coverage )
{
    std::string name = "test_name";
    std::unique_ptr<light_bulb> a = std::make_unique<light_bulb>(name, 13);
    a->on([](std::string name){puts(name.c_str());});
    std::unique_ptr<light_bulb> b = std::move(a);
    EXPECT_EQ(b->getStatus(), STATE::ON);

    light_bulb lb1(name,14);
    light_bulb lb2(lb1);

    EXPECT_EQ(lb2.getStatus(), STATE::UNKNOWN);
    lb1.off([](std::string name){puts(name.c_str());});
    EXPECT_EQ(lb1.getStatus(), STATE::OFF);
    EXPECT_EQ(lb2.getStatus(), STATE::UNKNOWN);

    lb1 = lb2;
    EXPECT_EQ(lb1.getStatus(), STATE::UNKNOWN);
}

TEST_F(light_house_fixture, dump )
{
    house_lighting_handler lHandler;
    std::string nameRoom = "kuchnia";
    std::string nameBulb1 = "glowna";
    std::string nameBulb2 = "kiniet";

    lHandler.addRoom(nameRoom);
    lHandler.addBulbInRoom(nameRoom, nameBulb1,11);
    lHandler.addBulbInRoom(nameRoom, nameBulb2,12);

    std::cout << iDom_API::getDump() << std::endl;

}

TEST_F(light_house_fixture, add_new_room_and_2_bulb )
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

    lHandler.m_lightingBulbMap[11]->off([](std::string name){puts(name.c_str());});
    EXPECT_EQ(STATE::OFF, lHandler.m_lightingBulbMap[11]->getStatus());
}
