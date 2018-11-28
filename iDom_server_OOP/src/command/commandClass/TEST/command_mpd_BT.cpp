#include "../command_mpd.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_mpd_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_mpd_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_mpd> test_command_mpd;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_mpd = std::make_unique <command_mpd> ("mpd");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_mpd_Class_fixture, unknownParameter)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("test");
    auto ret = test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "unknown parameter test");

 }

TEST_F(command_mpd_Class_fixture, list)
{
    test_my_data.ptr_MPD_info->songList = {"song 1","song 2","song 3"};
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("list");
    auto ret = test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "song 1\nsong 2\nsong 3\n");
 }

TEST_F(command_mpd_Class_fixture, get)
{
    test_my_data.ptr_MPD_info->volume = 33;
    test_my_data.ptr_MPD_info->radio = "radio test";
    test_my_data.ptr_MPD_info->title = "title test";

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("get");
    test_v.push_back("volume");

    auto ret = test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "33");

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("get");
    test_v.push_back("info");

    ret = test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "radio test : title test");
 }
TEST_F(command_mpd_Class_fixture, volume)
{
    test_my_data.ptr_MPD_info->volume = 33;

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("volume");
    test_v.push_back("up");

    blockQueue test_q;
    test_q._clearAll();

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLUP);

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("volume");
    test_v.push_back("down");

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLDOWN);

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("volume");
    test_v.push_back("55");

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLSET);
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, 55);
    /////// voule -gt 100%
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("volume");
    test_v.push_back("155");

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::NULL_);
    EXPECT_EQ(test_my_data.ptr_MPD_info->volume, 55);
 }

TEST_F(command_mpd_Class_fixture, pause)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("pause");

    blockQueue test_q;
    test_q._clearAll();

    auto ret = test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PAUSE);
    EXPECT_STREQ(ret.c_str(), "paused!");
}

TEST_F(command_mpd_Class_fixture, next)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("next");

    blockQueue test_q;
    test_q._clearAll();

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::NEXT);
}

TEST_F(command_mpd_Class_fixture, prev)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("prev");

    blockQueue test_q;
    test_q._clearAll();

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PREV);
}

TEST_F(command_mpd_Class_fixture, stop)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("stop");

    blockQueue test_q;
    test_q._clearAll();

    auto ret = test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
    EXPECT_STREQ(ret.c_str(), "stoped!");
}

TEST_F(command_mpd_Class_fixture, play_playID)
{
    test_my_data.idom_all_state.houseState = STATE::UNLOCK;
    test_my_data.ptr_MPD_info->currentSongID = 2;
    test_my_data.ptr_MPD_info->songList = {"song 1","song 2","song 3"};

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("start");

    blockQueue test_q;
    test_q._clearAll();

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("start");
    test_v.push_back("3");

    test_q._clearAll();

    test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY_ID);
    EXPECT_EQ(test_my_data.ptr_MPD_info->currentSongID, 3);
}
