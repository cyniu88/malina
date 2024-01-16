#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_ardu.h"
#include "../../../blockQueue/blockqueue.h"

class commandArdu_Class_fixture : public testing::Test
{
public:
    commandArdu_Class_fixture()
    {
        test_q._clearAll();
        test_server_settings._rflink.RFLinkPort = "testPort";
        test_server_settings._server.radio433MHzConfigFile = "/mnt/ramdisk/433_eq_conf.json";
        test_context.server_settings = &test_server_settings;
        test_v.push_back("433MHz");
        test_context.main_REC = test_rec;
        test_context.main_RFLink = std::make_shared<RFLinkHandler>(&test_context);
        test_rec = std::make_shared<RADIO_EQ_CONTAINER>(&test_context);
        test_rec->loadConfig(test_server_settings._server.radio433MHzConfigFile);
        test_context.main_REC = test_rec;
        test_ardu = std::make_unique<command_ardu>("ardu", &test_context);
    }

protected:
    std::vector<std::string> test_v= {"ardu"};
    blockQueue test_q;
    std::unique_ptr<command_ardu> test_ardu;
    thread_data test_context;
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec;
    CONFIG_JSON test_server_settings;
};

TEST_F(commandArdu_Class_fixture, wrongMSGformat)
{
    test_v.push_back("EV1527;ID=01e7be;SWITCH=01;CMD=ON;"); // wronh msg format missing 20;
    EXPECT_THROW(test_ardu->execute(test_v, &test_context), WRONG_FORMAT);
}

TEST_F(commandArdu_Class_fixture, weatherStationTemp)
{
    test_v.push_back("20;2A;LaCrosse;ID=0704;TEMP=8043;");
    test_ardu->execute(test_v, &test_context);
    RADIO_WEATHER_STATION* st = static_cast<RADIO_WEATHER_STATION*>(test_context.main_REC->getEqPointer("first"));
    EXPECT_DOUBLE_EQ(-6.7, st->data.getTemperature().value() );
    EXPECT_FALSE(st->data.getHumidity().has_value() );
    test_v[2] = "20;35;LaCrosse;ID=0704;HUM=42;";
    test_ardu->execute(test_v, &test_context);
    EXPECT_DOUBLE_EQ(42, st->data.getHumidity().value() );
    test_v[2] = "20;2A;LaCrosse;ID=0704;TEMP=0000;";
    test_ardu->execute(test_v, &test_context);
    EXPECT_DOUBLE_EQ(0, st->data.getTemperature().value() );
}

TEST_F(commandArdu_Class_fixture, button)
{
    test_v.push_back("20;2A;LaCrosse;ID=458;");
    test_ardu->execute(test_v, &test_context);

}

TEST_F(commandArdu_Class_fixture, command_ardu_show)
{
    test_v.pop_back();
    test_v.push_back("show");
    std::string retStr = test_ardu->execute(test_v, &test_context);
    EXPECT_THAT(retStr, testing::HasSubstr("data: 0"));
    EXPECT_THAT(retStr, testing::HasSubstr("temperature= 0c"));
}

TEST_F(commandArdu_Class_fixture, command_ardu_help)
{
    command_ardu test_Command_ardu ("ardu");

    std::string retStr = test_Command_ardu.help();
    EXPECT_STREQ(retStr.c_str(), " only for internal usege\n");
}

TEST_F(commandArdu_Class_fixture, command_ardu_433MHz_throw)
{
    test_v.push_back("fake_msg");
    std::string retStr = test_ardu->execute(test_v, &test_context);

    EXPECT_THAT(retStr, testing::HasSubstr("for"));
}

TEST_F(commandArdu_Class_fixture, command_ardu_433MHz_OK)
{
    test_v.push_back("20;53;OK;");
    test_ardu->execute(test_v, &test_context);
    EXPECT_NE(test_context.main_RFLink->m_okTime, 0);
    EXPECT_EQ(test_context.main_RFLink->m_okTime, Clock::getUnixTime());
}

TEST_F(commandArdu_Class_fixture, command_ardu_433MHz_PING)
{
    test_v.push_back("20;99;PONG;");
    test_ardu->execute(test_v, &test_context);
    EXPECT_NE(test_context.main_RFLink->m_pingTime, 0);
    EXPECT_EQ(test_context.main_RFLink->m_pingTime, Clock::getUnixTime());
}
