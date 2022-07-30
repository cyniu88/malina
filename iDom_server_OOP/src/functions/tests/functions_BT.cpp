#include <gtest/gtest.h>
#include "../functions.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../iDomTools/mock/iDomToolsMock.h"

class functions_fixture : public testing::Test
{
protected:
functions_fixture():
    main_iDomTools(std::make_shared<iDomToolsMock>())
{
    useful_F::myStaticData = &test_my_data;
    test_my_data.main_iDomTools  = main_iDomTools;
    test_my_data.server_settings = &test_server_set;
}
    thread_data test_my_data;
    CONFIG_JSON test_server_set;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
};

void useful_F::sleep(std::chrono::seconds sec)
{
    std::cout << "sleep() sec: " << sec.count() << std::endl;
}

TEST(functions_, tokenizer)
{
     std::string test_msg = "one=two three";
     std::vector<std::string> test_v;

     EXPECT_EQ(test_v.size(), 0);
     useful_F::tokenizer(test_v,"= ",test_msg);

     EXPECT_EQ(test_v.size(),3);
     EXPECT_STREQ(test_v.at(2).c_str(),"three");
}

TEST(functions_, sekToUptime)
{
    EXPECT_STREQ(useful_F::sek_to_uptime(34534).c_str(),"\n0 day 9 hours 35 minutes 34 seconds");
}

TEST(functions_, tokenizerEmpty)
{
     std::string test_msg = "";
     std::vector<std::string> test_v;

     EXPECT_EQ(test_v.size(), 0);
     EXPECT_THROW(useful_F::tokenizer(test_v,"= ",test_msg),std::string);
}

TEST(functions_, removeHtmlTag)
{
     std::string test_msg = "<html>test</html>";
     std::string test_pure_str = useful_F_libs::removeHtmlTag(test_msg);

     EXPECT_STREQ(test_pure_str.c_str(),"test");
}

TEST(functions_, repalceAll)
{
     std::string test_msg = "one two three";
     std::string test_pure_str = useful_F_libs::replaceAll(test_msg,"two","zero");

     EXPECT_STREQ(test_pure_str.c_str(),"one zero three");
}

TEST(functions_, split)
{
    std::string test_msg = "one two three";
    std::vector<std::string> test_v;

    EXPECT_EQ(test_v.size(), 0);
    test_v = useful_F::split(test_msg,' ');

    EXPECT_EQ(test_v.size(),3);
    EXPECT_STREQ(test_v.at(2).c_str(),"three");
}

TEST(functions_, RSHash)
{
    std::string msg = "test msg";
    std::string s1 = useful_F::RSHash(msg, 33, 44);
    std::string s2 = useful_F::RSHash(msg, 33 ,44);
    EXPECT_STREQ(s1.c_str(), s2.c_str());

    s1 = useful_F::RSHash(msg, 35, 44);
    s2 = useful_F::RSHash(msg, 33 ,44);
    EXPECT_STRNE(s1.c_str(), s2.c_str());
}

TEST_F(functions_fixture, setStaticData)
{
    test_my_data.sleeper = 99;
    EXPECT_EQ( useful_F::myStaticData->sleeper, 99 );
    thread_data test_my_data2;
    test_my_data2.sleeper = 88;
    EXPECT_EQ( test_my_data2.sleeper, 88 );
    EXPECT_EQ( useful_F::myStaticData->sleeper, 99 );
    useful_F::setStaticData(&test_my_data2);
    EXPECT_EQ( useful_F::myStaticData->sleeper, 88 );
}

TEST_F(functions_fixture, sleepThread)
{
    EXPECT_CALL(*main_iDomTools.get(), ledClear(testing::_,testing::_)).Times(testing::AnyNumber());
    EXPECT_CALL(*main_iDomTools.get(), ledOFF());
    EXPECT_CALL(*main_iDomTools.get(), MPD_stop());
    EXPECT_CALL(*main_iDomTools.get(), turnOff433MHzSwitch("listwa"));
    std::array <Thread_array_struc,10> test_THRARRSTR;
    test_my_data.main_THREAD_arr = &test_THRARRSTR;

    test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();
    test_my_data.ptr_MPD_info->volume = 3;

   // RADIO_EQ_CONTAINER_STUB test_rec(&test_my_data);
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec = std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data);
    test_rec->loadConfig(test_server_set._server.radio433MHzConfigFile);
    test_my_data.main_REC = (test_rec);
    test_my_data.alarmTime.time = Clock::getTime();
    test_my_data.alarmTime.state = STATE::ACTIVE;

    test_my_data.sleeper = 10;

    useful_F::sleeper_mpd(&test_my_data,"test sleep");
}

TEST_F(functions_fixture, json_config)
{
    // read a JSON file
    std::ifstream i("../config/config_iDom.json");
    nlohmann::json j;
    i >> j;
    auto ret = useful_F::configJsonFileToStruct(j);

    EXPECT_EQ(ret._server.PORT, 8833);

    EXPECT_TRUE(ret._server.encrypted);

    EXPECT_EQ(ret._fb_viber.viberReceiver.size(),2);
    EXPECT_STREQ(ret._fb_viber.viberReceiver.at(1).c_str(),
                 "5");
    EXPECT_TRUE(ret._runThread.MPD);
    EXPECT_TRUE(ret._runThread.DUMMY);
    EXPECT_TRUE(ret._runThread.CRON);
    EXPECT_TRUE(ret._runThread.RFLink);
    EXPECT_TRUE(ret._runThread.RS232);
    EXPECT_FALSE(ret._runThread.MQTT);
}


TEST_F(functions_fixture, Server_connectivity_thread)
{
    test_my_data.encriptionKey = "key";
    test_my_data.server_settings->_server.encrypted = true;
    test_my_data.from.sin_addr = {22};
   // useful_F::Server_connectivity_thread(&test_my_data, "fake");
}
