#include "../commandhandlerrs232.h"
#include "../commandhandlermqtt.h"
#include "../commandhandlergateway.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

class command_handler_rs232_fixture : public iDomTOOLS_ClassTest
{
public:
    command_handler_rs232_fixture()
    {
    }

protected:
    std::unique_ptr<commandHandlerRS232> test_chRS232;
    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_chRS232 = std::make_unique<commandHandlerRS232>(&test_my_data);
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_handler_rs232_fixture, main)
{
    test_v.push_back("ok");
    auto ret = test_chRS232->run(test_v, &test_my_data);
    EXPECT_STREQ(ret.c_str(), "\nEND\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
class command_handler_mqtt_fixture : public iDomTOOLS_ClassTest
{
public:
    command_handler_mqtt_fixture()
    {
    }

protected:
    std::unique_ptr<CommandHandlerMQTT> test_chMQTT;
    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_chMQTT = std::make_unique<CommandHandlerMQTT>();
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_handler_mqtt_fixture, main)
{
    test_v.push_back("wifi");
    auto ret = test_chMQTT->run(test_v, &test_my_data);
    EXPECT_STREQ(ret.c_str(), "done");
}

TEST_F(command_handler_mqtt_fixture, unknown_command)
{
    time(&test_my_data.start);
    test_v.clear();
    test_v.push_back("fake");
    auto ret = test_chMQTT->run(test_v, &test_my_data);
    std::cout << "DATA: " << ret << std::endl;
    EXPECT_THAT(ret, testing::HasSubstr("unknown") );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
class command_handler_gateway_fixture : public iDomTOOLS_ClassTest
{
public:
    command_handler_gateway_fixture()
    {
    }

protected:
    std::unique_ptr<commandHandlerGATEWAY> test_chGATEWAY;
    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_chGATEWAY = std::make_unique<commandHandlerGATEWAY>(&test_my_data);
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_handler_gateway_fixture, not_enough_parameters)
{
    time(&test_my_data.start);
    test_v.clear();
    test_v.push_back("fake");
    auto ret = test_chGATEWAY->run(test_v, &test_my_data);
    std::cout << "DATA: " << ret << std::endl;
    EXPECT_THAT(ret, testing::HasSubstr("not enough parameters") );
}
