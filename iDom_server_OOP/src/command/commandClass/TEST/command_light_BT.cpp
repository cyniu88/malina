#include "../command_light.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_light_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_light_Class_fixture()
    {

    }

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

    EXPECT_THAT(ret, testing::HasSubstr("bulb ID\": 126,"));
}
