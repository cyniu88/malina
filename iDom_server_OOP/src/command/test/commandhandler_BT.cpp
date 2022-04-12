#include<gtest/gtest.h>

#include "../commandhandlerrs232.h"
#include "../commandhandlermqtt.h"
#include "../commandhandlergateway.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../iDomTools/mock/iDomToolsMock.h"
#include "../../iDomTools/test/iDomTools_fixture.h"
#include "../../HOUSE/house_room_handler.h"
#include "../../functions/functions.h"

class command_handler_main : public testing::Test
{
public:
    command_handler_main():
        test_rec(std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data)),
        main_iDomTools(std::make_shared<iDomToolsMock>()),
        testRoomHandler(std::make_shared<house_room_handler>(&test_my_data))
    {
        test_server_settings._server.radio433MHzConfigFile = "/mnt/ramdisk/433_eq_conf.json";
        test_my_data.server_settings = &test_server_settings;
        test_rec->loadConfig(test_server_settings._server.radio433MHzConfigFile);
        test_my_data.main_REC = test_rec;
        test_my_data.main_iDomTools = main_iDomTools;
        testRoomHandler->loadConfig("../config/bulb_config.json");
        test_my_data.main_house_room_handler = testRoomHandler;
        useful_F::myStaticData = &test_my_data;
    }
    thread_data test_my_data;
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec;
    CONFIG_JSON test_server_settings;
    std::vector<std::string> test_v;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    std::shared_ptr<house_room_handler> testRoomHandler;
};

class command_handler_rs232_fixture : public command_handler_main
{
public:
    command_handler_rs232_fixture():test_chRS232(std::make_unique<commandHandlerRS232>(&test_my_data))
    {
    }

protected:
    std::unique_ptr<commandHandlerRS232> test_chRS232;
};

TEST_F(command_handler_rs232_fixture, main)
{
    test_v.push_back("ok");
    auto ret = test_chRS232->run(test_v, &test_my_data);
    EXPECT_STREQ(ret.c_str(), "\nEND\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
class command_handler_mqtt_fixture : public command_handler_main
{
public:
    command_handler_mqtt_fixture():test_chMQTT(std::make_unique<CommandHandlerMQTT>())
    {
    }

protected:
    std::unique_ptr<CommandHandlerMQTT> test_chMQTT;
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

TEST_F(command_handler_mqtt_fixture, voice_mqtt)
{
    time(&test_my_data.start);
    test_v.clear();
    test_v.push_back("radIo");
    test_v.push_back("włąCz");
    EXPECT_CALL(*main_iDomTools.get(), MPD_play(testing::_));
    auto ret = test_chMQTT->run(test_v, &test_my_data);
    std::cout << "DATA: " << ret << std::endl;
    EXPECT_THAT(ret, testing::HasSubstr("done") );

    test_v.clear();
    test_v.push_back("światła");
    test_v.push_back("wszędzie");
    test_v.push_back("wyłąCz");
    test_chMQTT->run(test_v, &test_my_data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
class command_handler_gateway_fixture : public command_handler_main
{
public:
    command_handler_gateway_fixture():test_chGATEWAY(std::make_unique<commandHandlerGATEWAY>(&test_my_data))
    {
    }

protected:
    std::unique_ptr<commandHandlerGATEWAY> test_chGATEWAY;
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
