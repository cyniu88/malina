#include <gtest/gtest.h>
#include "../command_ardu.h"
#include "../../../functions/functions.h"
#include "../../../RADIO_433_eq/radio_433_eq.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"

TEST(commandArdu_Class, wrongMSGformat)
{
    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_rec.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    test_my_data.main_REC = (&test_rec);

    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;
    useful_F::myStaticData = &test_my_data;

    RFLinkHandler test_RFLink(&test_my_data);
    test_my_data.main_RFLink = &test_RFLink;
    command_ardu test_ardu("ardu", &test_my_data);
    std::vector<std::string> test_v= {"ardu"};
    test_v.push_back("433MHz");
    test_v.push_back("EV1527;ID=01e7be;SWITCH=01;CMD=ON;"); // wronh msg format missing 20;
    EXPECT_THROW(test_ardu.execute(test_v, &test_my_data), WRONG_FORMAT);
}

TEST(commandArdu_Class, UnlockHome)
{
    blockQueue test_q;
    test_q._clearAll();

    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_rec.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    test_my_data.main_REC = (&test_rec);

    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;
    useful_F::myStaticData = &test_my_data;

    RFLinkHandler test_RFLink(&test_my_data);
    test_my_data.main_RFLink = &test_RFLink;

    test_idomTOOLS.lockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);

    command_ardu test_ardu("ardu", &test_my_data);
    std::vector<std::string> test_v= {"ardu"};
    test_v.push_back("433MHz");
    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");
    test_ardu.execute(test_v, &test_my_data);
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);


    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);

}

TEST(commandArdu_Class, LockHome)
{
    blockQueue test_q;
    test_q._clearAll();

    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_rec.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    test_my_data.main_REC = (&test_rec);

    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;
    useful_F::myStaticData = &test_my_data;

    RFLinkHandler test_RFLink(&test_my_data);
    test_my_data.main_RFLink = &test_RFLink;

    test_idomTOOLS.unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);

    command_ardu test_ardu("ardu", &test_my_data);
    std::vector<std::string> test_v= {"ardu"};
    test_v.push_back("433MHz");
    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");
    for(auto i : {1,2,3}){
        test_ardu.execute(test_v, &test_my_data);
    }

    EXPECT_EQ(test_status.getObjectState("house"),STATE::LOCK);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
}

TEST(commandArdu_Class, playMusic)
{
    blockQueue test_q;
    test_q._clearAll();

    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_rec.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    test_my_data.main_REC = (&test_rec);

    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;
    useful_F::myStaticData = &test_my_data;

    RFLinkHandler test_RFLink(&test_my_data);
    test_my_data.main_RFLink = &test_RFLink;

    test_idomTOOLS.unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);
    test_status.addObject("music",STATE::STOP);

    command_ardu test_ardu("ardu", &test_my_data);
    std::vector<std::string> test_v= {"ardu"};
    test_v.push_back("433MHz");
    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");

    test_ardu.execute(test_v, &test_my_data);

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(),0);
}

TEST(commandArdu_Class, stopMusic)
{
    blockQueue test_q;
    test_q._clearAll();

    thread_data test_my_data;

    RADIO_EQ_CONTAINER test_rec(&test_my_data);
    test_rec.loadConfig("/etc/config/iDom_SERVER/433_eq.conf");
    test_my_data.main_REC = (&test_rec);

    config test_server_set;
    test_server_set.TS_KEY = "key test";
    test_server_set.viberSender = "test sender";
    test_server_set.viberReceiver = {"R1","R2"};
    test_my_data.server_settings = &test_server_set;

    iDomSTATUS test_status;
    test_status.addObject("house");
    test_my_data.main_iDomStatus = &test_status;

    iDOM_STATE main_iDomStatus;
    test_my_data.idom_all_state = main_iDomStatus;

    iDomTOOLS test_idomTOOLS(&test_my_data);

    test_my_data.main_iDomTools = &test_idomTOOLS;
    useful_F::myStaticData = &test_my_data;

    RFLinkHandler test_RFLink(&test_my_data);
    test_my_data.main_RFLink = &test_RFLink;

    test_idomTOOLS.unlockHome();
    EXPECT_EQ(test_status.getObjectState("house"),STATE::UNLOCK);
    test_status.addObject("music",STATE::PLAY);

    command_ardu test_ardu("ardu", &test_my_data);
    std::vector<std::string> test_v= {"ardu"};
    test_v.push_back("433MHz");
    test_v.push_back("20;EV1527;ID=01e7be;SWITCH=01;CMD=ON;");

    test_ardu.execute(test_v, &test_my_data);

    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_EQ(test_q._size(),0);
}
