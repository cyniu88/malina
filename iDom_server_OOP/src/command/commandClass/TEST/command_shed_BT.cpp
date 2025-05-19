#include<gtest/gtest.h>
#include<gmock/gmock.h>

#include "../command_shed.h"
#include "../../iDom_server_OOP/src/iDomTools/mock/iDomToolsMock.h"

class command_shed_Class_fixture : public testing::Test
{
public:
    command_shed_Class_fixture():test_command_shed(std::make_unique <command_shed>("shed")),
        main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.m_keyHandler = std::make_unique<iDomKEY_ACCESS>("");
        test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("iDomSERVER test");
        test_context.main_iDomTools = main_iDomTools;
    }

protected:
    std::unique_ptr<command_shed> test_command_shed;

    std::vector<std::string> test_v;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    thread_context test_context;
};

TEST_F(command_shed_Class_fixture, wrongParameter)
{
    test_v.push_back("shed");
    test_v.push_back("t");
    auto ret = test_command_shed->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "command shed - wrong paramiter");
}

TEST_F(command_shed_Class_fixture, put_get_json)
{

    test_context.lusina.shedConfJson = nlohmann::json::parse(R"({
       "deepSleep":true,
       "howLongDeepSleep":177,
       "fanON":false
   })");
    EXPECT_CALL(*main_iDomTools.get(), isItDay()).WillOnce(testing::Return(true));
    std::string msg = R"({"acdc":2,"temperatura":21.37999916,"ciśnienie":992.303772,"wilgotność":51.86914063,"millis":119626,"bateria":3.900585938, "podłoga": 22.6})";
    test_v.push_back("shed");
    test_v.push_back("put");
    test_v.push_back(msg);
    auto ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "DONE");

    test_v.clear();
    test_v.push_back("shed");
    test_v.push_back("show");
    ret = test_command_shed->execute(test_v, &test_context);
    std::cout << ret << std::endl;
}

TEST_F(command_shed_Class_fixture, null_in_json)
{

    test_context.lusina.shedConfJson = nlohmann::json::parse(R"({
       "deepSleep":true,
       "howLongDeepSleep":177,
       "fanON":false
   })");
    EXPECT_CALL(*main_iDomTools.get(), isItDay()).WillOnce(testing::Return(true));
    std::string msg = R"({"acdc":2,"temperatura":null,"ciśnienie":992.303772,"wilgotność":51.86914063,"millis":119626,"bateria":3.900585938, "podłoga": 22.6})";
    test_v.push_back("shed");
    test_v.push_back("put");
    test_v.push_back(msg);
    auto ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "DONE");

    test_v.clear();
    test_v.push_back("shed");
    test_v.push_back("show");
    ret = test_command_shed->execute(test_v, &test_context);
    std::cout << ret << std::endl;
}
