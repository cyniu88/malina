#include "../command_light.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_light_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_light_Class_fixture(){}

protected:
    std::unique_ptr<command_light> test_command_light;
    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        std::string cfg("../config/bulb_config.json");
        test_command_light = std::make_unique <command_light> ("light");
        test_my_data.main_house_lighting_handler = std::make_shared<house_lighting_handler>(&test_my_data);
        test_my_data.main_house_lighting_handler->loadConfig(cfg);
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_light_Class_fixture, mqtt_bulb_state_update)
{
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;111;30;1\n");
    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_lighting_handler->m_lightingBulbMap[111]->getStatus(), STATE::ON);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;111;30;0\n");
    ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_lighting_handler->m_lightingBulbMap[111]->getStatus(), STATE::OFF);
}

TEST_F(command_light_Class_fixture, mqtt_bulb_state_update_bulb_not_exist)
{
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;330;30;1\n");
    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_lighting_handler->m_lightingBulbMap[330]->getStatus(), STATE::ON);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;330;30;0\n");
    ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_lighting_handler->m_lightingBulbMap[330]->getStatus(), STATE::OFF);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("info");
    ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
}

TEST_F(command_light_Class_fixture, light_info)
{
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("info");

    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;

    EXPECT_THAT(ret, testing::HasSubstr(R"(bulb ID": 126,)"));
}

TEST_F(command_light_Class_fixture, light_info_on)
{
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("bulb");
    test_v.push_back("on");
    test_v.push_back("126");

    (void)test_command_light->execute(test_v,&test_my_data);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("info");
    test_v.push_back("on");

    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;

    EXPECT_THAT(ret, testing::HasSubstr(R"(bulb ID": 126,)"));
}

TEST_F(command_light_Class_fixture, on_off_bulb_command)
{
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("bulb");
    test_v.push_back("on");
    test_v.push_back("126");

    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::ON);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("bulb");
    test_v.push_back("off");
    test_v.push_back("126");

    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::OFF);
}

TEST_F(command_light_Class_fixture, on_off_all_bulbs_in_room_command)
{
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::UNDEFINE);
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(104)->getStatus(),
              STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("room");
    test_v.push_back("lazienka");
    test_v.push_back("on");

    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::ON);
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(104)->getStatus(),
              STATE::ON);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("room");
    test_v.push_back("lazienka");
    test_v.push_back("off");

    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::OFF);
    EXPECT_EQ(test_my_data.main_house_lighting_handler->m_lightingBulbMap.at(104)->getStatus(),
              STATE::OFF);
}

TEST_F(command_light_Class_fixture, on_off_all_bulbs_in_home_command)
{
    auto ret = test_my_data.main_house_lighting_handler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr("UNDEFINE"));

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("all");
    test_v.push_back("on");

    (void)test_command_light->execute(test_v,&test_my_data);
    ret = test_my_data.main_house_lighting_handler->getAllInfoJSON().dump(4);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, testing::Not(testing::HasSubstr(R"("STATUS": "OFF")")));
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("all");
    test_v.push_back("off");

    (void)test_command_light->execute(test_v,&test_my_data);
    ret = test_my_data.main_house_lighting_handler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::Not(testing::HasSubstr(R"("STATUS": "ON")")));
}

TEST_F(command_light_Class_fixture, wrong_param)
{
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("lazienka");
    test_v.push_back("off");

    EXPECT_NO_THROW( test_command_light->execute(test_v,&test_my_data));
}
