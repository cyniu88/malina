#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../command_ardu.h"
#include "../../../functions/functions.h"
#include "../../../RADIO_433_eq/radio_433_eq.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class commandArdu_Class_fixture : public iDomTOOLS_ClassTest
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
    command_ardu* test_ardu;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();

        test_q._clearAll();

        test_ardu = new command_ardu("ardu", &test_my_data);
        test_my_data.main_RFLink = std::make_shared<RFLinkHandler>(&test_my_data);;
        test_v.push_back("433MHz");
        std::cout << "commandArdu_Class_fixture SetUp" << std::endl;
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
        delete test_ardu;
        std::cout << "commandArdu_Class_fixture TearDown" << std::endl;
    }

};

TEST_F(commandArdu_Class_fixture, wrongMSGformat)
{
    test_v.push_back("EV1527;ID=01e7be;SWITCH=01;CMD=ON;"); // wronh msg format missing 20;
    EXPECT_THROW(test_ardu->execute(test_v, &test_my_data), WRONG_FORMAT);
}

TEST_F(commandArdu_Class_fixture, UnlockHome)
{
    test_my_data.main_iDomTools->lockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::LOCK);

    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");
    test_ardu->execute(test_v, &test_my_data);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNLOCK);

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
}

TEST_F(commandArdu_Class_fixture, LockHome)
{
    test_my_data.main_iDomTools->unlockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNLOCK);

    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");
    for(auto i = 0; i < 3; ++i){
        test_ardu->execute(test_v, &test_my_data);
    }

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::LOCK);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
}

TEST_F(commandArdu_Class_fixture, LockHome1unlockHome2)
{
    test_my_data.main_iDomTools->lockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::LOCK);

    ////////////////////////// unlock locker-main
    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");

    test_ardu->execute(test_v, &test_my_data);

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNLOCK);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);

    ////////////////////////// lock locker-2
    test_v.push_back("20;EV1527;ID=123456789;SWITCH=01;CMD=ON;");
    for(auto i = 0; i < 3; ++i){
        test_ardu->execute(test_v, &test_my_data);
    }

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::LOCK);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
}

TEST_F(commandArdu_Class_fixture, playMusic)
{
    test_my_data.main_iDomTools->unlockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNLOCK);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("listwa"),STATE::UNKNOWN);
    test_my_data.main_iDomStatus->addObject("music",STATE::STOP);

    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");

    test_ardu->execute(test_v, &test_my_data);

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("listwa"),STATE::ON);
}

TEST_F(commandArdu_Class_fixture, stopMusic)
{
    test_my_data.main_iDomTools->unlockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNLOCK);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("listwa"),STATE::UNKNOWN);
    test_my_data.main_iDomStatus->addObject("music",STATE::PLAY);

    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");

    test_ardu->execute(test_v, &test_my_data);

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("listwa"),STATE::OFF);
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

TEST_F(commandArdu_Class_fixture, NightLight)
{
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("Night_Light"),STATE::UNKNOWN);
    test_my_data.main_iDomTools->unlockHome();
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("house"),STATE::UNLOCK);
    test_v.push_back("20;EV1527;ID=458;SWITCH=01;CMD=ON;");

    test_ardu->execute(test_v, &test_my_data);

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("Night_Light"),STATE::ON);

    test_ardu->execute(test_v, &test_my_data);

    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("Night_Light"),STATE::OFF);
}
