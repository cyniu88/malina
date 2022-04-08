#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_ardu.h"
#include "../../../functions/functions.h"
#include "../../../RADIO_433_eq/radio_433_eq.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"

class commandArdu_Class_fixture : public testing::Test
{
public:
    commandArdu_Class_fixture() {
        this->test_RFLink = std::nullptr_t();
        this->test_ardu = std::nullptr_t();
    }

protected:
    std::vector<std::string> test_v= {"ardu"};
    RFLinkHandler* test_RFLink;
    blockQueue test_q;
    command_ardu* test_ardu; //TODO make smart PTR
    thread_data test_my_data;
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec;
    CONFIG_JSON test_server_settings;

    void SetUp() final
    {
        test_q._clearAll();
        test_server_settings._rflink.RFLinkPort = "testPort";
        test_server_settings._server.radio433MHzConfigFile = "/mnt/ramdisk/433_eq_conf.json";
        test_my_data.server_settings = &test_server_settings;
        test_v.push_back("433MHz");
        test_rec = std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data);
        test_rec->loadConfig(test_server_settings._server.radio433MHzConfigFile);
        test_my_data.main_REC = test_rec;
        test_my_data.main_RFLink = std::make_shared<RFLinkHandler>(&test_my_data);;
        test_ardu = new command_ardu("ardu", &test_my_data);
        std::cout << "commandArdu_Class_fixture SetUp" << std::endl;
    }

    void TearDown() final
    {
        delete test_ardu;
        std::cout << "commandArdu_Class_fixture TearDown" << std::endl;
    }

};

TEST_F(commandArdu_Class_fixture, wrongMSGformat)
{
    test_v.push_back("EV1527;ID=01e7be;SWITCH=01;CMD=ON;"); // wronh msg format missing 20;
    EXPECT_THROW(test_ardu->execute(test_v, &test_my_data), WRONG_FORMAT);
}

TEST_F(commandArdu_Class_fixture, weatherStationTemp)
{
    test_v.push_back("20;2A;LaCrosse;ID=0704;TEMP=8043;");
    test_ardu->execute(test_v, &test_my_data);
    RADIO_WEATHER_STATION* st = static_cast<RADIO_WEATHER_STATION*>(test_my_data.main_REC->getEqPointer("first"));
    EXPECT_DOUBLE_EQ(-6.7, st->data.getTemperature() );
    EXPECT_DOUBLE_EQ(0, st->data.getHumidity() );
    test_v[2] = "20;35;LaCrosse;ID=0704;HUM=42;";
    test_ardu->execute(test_v, &test_my_data);
    EXPECT_DOUBLE_EQ(42, st->data.getHumidity() );
    test_v[2] = "20;2A;LaCrosse;ID=0704;TEMP=0000;";
    test_ardu->execute(test_v, &test_my_data);
    EXPECT_DOUBLE_EQ(0, st->data.getTemperature() );
}

TEST_F(commandArdu_Class_fixture, button)
{
    test_v.push_back("20;2A;LaCrosse;ID=458;");
    test_ardu->execute(test_v, &test_my_data);

}

TEST_F(commandArdu_Class_fixture, command_ardu_show)
{
    test_v.pop_back();
    test_v.push_back("show");
    std::string retStr = test_ardu->execute(test_v, &test_my_data);
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
    std::string retStr = test_ardu->execute(test_v, &test_my_data);

    EXPECT_THAT(retStr, testing::HasSubstr("for"));
}

TEST_F(commandArdu_Class_fixture, command_ardu_433MHz_OK)
{
    test_v.push_back("20;53;OK;");
    test_ardu->execute(test_v, &test_my_data);
    EXPECT_NE(test_my_data.main_RFLink->m_okTime, 0);
    EXPECT_EQ(test_my_data.main_RFLink->m_okTime, Clock::getUnixTime());
}

TEST_F(commandArdu_Class_fixture, command_ardu_433MHz_PING)
{
    test_v.push_back("20;99;PONG;");
    test_ardu->execute(test_v, &test_my_data);
    EXPECT_NE(test_my_data.main_RFLink->m_pingTime, 0);
    EXPECT_EQ(test_my_data.main_RFLink->m_pingTime, Clock::getUnixTime());
}
