#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_light.h"
#include "../../../functions/functions.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"

class command_light_Class_fixture : public testing::Test
{
public:
    command_light_Class_fixture():
        test_command_light (std::make_unique <command_light>("light")),
        main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        std::string cfg("../config/bulb_config.json");;
        test_my_data.main_house_room_handler = std::make_shared<house_room_handler>(&test_my_data);
        test_my_data.main_house_room_handler->loadConfig(cfg);
        useful_F::myStaticData = &test_my_data;
        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
        test_my_data.main_iDomTools = main_iDomTools;
        test_my_data.server_settings = &test_server_settings;
        test_my_data.server_settings->_fb_viber.viberSender = "test sender";
        test_my_data.server_settings->_fb_viber.viberReceiver = {"R1","R2"};
    }

protected:
    std::unique_ptr<command_light> test_command_light;
    std::vector<std::string> test_v;
    thread_data test_my_data;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    CONFIG_JSON test_server_settings;
};

TEST_F(command_light_Class_fixture, mqtt_bulb_state_update)
{
    EXPECT_CALL(*main_iDomTools.get(),
                sendViberMsg("zmiana statusu lampy jedna w pomieszczeniu: pokoj dzieci na ON przyciskiem: 30 czas trwania: 00:00",
                             testing::_,testing::_,testing::_,testing::_));
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;226;30;1\n");
    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_room_handler->m_lightingBulbMap[226]->getStatus(), STATE::ON);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;226;30;0\n");
    ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_room_handler->m_lightingBulbMap[226]->getStatus(), STATE::OFF);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("info");

    ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
}

TEST_F(command_light_Class_fixture, mqtt_bulb_state_update_bulb_not_exist)
{
    EXPECT_CALL(*main_iDomTools.get(),
                sendViberMsg("zmiana statusu lampy bulbName w pomieszczeniu: roomName na ON przyciskiem: 30 czas trwania: 00:00",
                             testing::_,testing::_,testing::_,testing::_));
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;330;30;1\n");
    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_room_handler->m_lightingBulbMap[330]->getStatus(), STATE::ON);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;330;30;0\n");
    ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
    EXPECT_EQ( test_my_data.main_house_room_handler->m_lightingBulbMap[330]->getStatus(), STATE::OFF);
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
    EXPECT_CALL(*main_iDomTools.get(),
                sendViberMsg("zmiana statusu lampy wanna w pomieszczeniu: lazienka na ON przyciskiem: 30 czas trwania: 00:00",
                             testing::_,testing::_,testing::_,testing::_));
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;126;30;1\n");
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
    EXPECT_CALL(*main_iDomTools.get(),
                sendViberMsg("zmiana statusu lampy wanna w pomieszczeniu: lazienka na ON przyciskiem: 30 czas trwania: 00:00",
                             testing::_,testing::_,testing::_,testing::_));
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("bulb");
    test_v.push_back("on");
    test_v.push_back("126");

    (void)test_command_light->execute(test_v,&test_my_data);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;126;30;1\n");
    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::ON);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("bulb");
    test_v.push_back("off");
    test_v.push_back("126");

    (void)test_command_light->execute(test_v,&test_my_data);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;126;30;0\n");
    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::OFF);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("info");

    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
}

TEST_F(command_light_Class_fixture, change_bulb_command)
{
    EXPECT_CALL(*main_iDomTools.get(),
                sendViberMsg("zmiana statusu lampy wanna w pomieszczeniu: lazienka na ON przyciskiem: 30 czas trwania: 00:00",
                             testing::_,testing::_,testing::_,testing::_));
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("bulb");
    test_v.push_back("change");
    test_v.push_back("126");

    (void)test_command_light->execute(test_v,&test_my_data);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;126;30;1\n");
    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::ON);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("bulb");
    test_v.push_back("change");
    test_v.push_back("126");

    (void)test_command_light->execute(test_v,&test_my_data);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("state;126;30;0\n");
    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::OFF);
}

TEST_F(command_light_Class_fixture, on_off_all_bulbs_in_room_command)
{
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(226)->getStatus(),
              STATE::UNDEFINE);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(227)->getStatus(),
              STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("room");
    test_v.push_back("lazienka");
    test_v.push_back("on");

    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::ACTIVE);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(127)->getStatus(),
              STATE::ACTIVE);

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("room");
    test_v.push_back("lazienka");
    test_v.push_back("off");

    (void)test_command_light->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(126)->getStatus(),
              STATE::DEACTIVE);
    EXPECT_EQ(test_my_data.main_house_room_handler->m_lightingBulbMap.at(127)->getStatus(),
              STATE::DEACTIVE);
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("info");

    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
}

TEST_F(command_light_Class_fixture, on_off_all_bulbs_in_home_command)
{
    auto ret = test_my_data.main_house_room_handler->getAllInfoJSON().dump(4);
    EXPECT_THAT(ret, testing::HasSubstr("UNDEFINE"));

    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("all");
    test_v.push_back("on");

    (void)test_command_light->execute(test_v,&test_my_data);
    ret = test_my_data.main_house_room_handler->getAllInfoJSON().dump(4);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, testing::Not(testing::HasSubstr(R"("STATUS": "OFF")")));
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("all");
    test_v.push_back("off");

    (void)test_command_light->execute(test_v,&test_my_data);
    ret = test_my_data.main_house_room_handler->getAllInfoJSON().dump(4);
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

TEST_F(command_light_Class_fixture, room_stats)
{
    test_v.clear();
    test_v.push_back("light");
    test_v.push_back("room");
    test_v.push_back("stats");

    std::cout << test_command_light->execute(test_v,&test_my_data) << std::endl;
}
