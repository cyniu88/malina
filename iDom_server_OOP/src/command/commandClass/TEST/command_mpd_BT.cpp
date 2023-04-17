#include <gtest/gtest.h>

#include "../command_mpd.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"


class command_mpd_Class_fixture : public testing::Test
{
public:
    command_mpd_Class_fixture():
        test_command_mpd(std::make_unique <command_mpd> ("mpd")),
        main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_my_data.main_iDomTools = main_iDomTools;
        test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();
    }

protected:
    std::unique_ptr<command_mpd> test_command_mpd;
    std::vector<std::string> test_v;
    thread_data test_my_data;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
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

    EXPECT_CALL(*main_iDomTools.get(), MPD_volumeUp());

    test_command_mpd->execute(test_v,&test_my_data);

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("volume");
    test_v.push_back("down");

    EXPECT_CALL(*main_iDomTools.get(), MPD_volumeDown());
    test_command_mpd->execute(test_v,&test_my_data);

    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("volume");
    test_v.push_back("55");


    EXPECT_CALL(*main_iDomTools.get(), MPD_volumeSet(testing::_, 55));
    test_command_mpd->execute(test_v,&test_my_data);
    /////// voule -gt 100%
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("volume");
    test_v.push_back("155");

    test_command_mpd->execute(test_v,&test_my_data);
 }

TEST_F(command_mpd_Class_fixture, pause)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("pause");

    EXPECT_CALL(*main_iDomTools.get(), MPD_pause());

    auto ret = test_command_mpd->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "paused!");
}

TEST_F(command_mpd_Class_fixture, next)
{
    EXPECT_CALL(*main_iDomTools, MPD_next());
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("next");

    test_command_mpd->execute(test_v,&test_my_data);
}

TEST_F(command_mpd_Class_fixture, prev)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("prev");

    EXPECT_CALL(*main_iDomTools.get(), MPD_prev());

    test_command_mpd->execute(test_v,&test_my_data);
}

TEST_F(command_mpd_Class_fixture, stop)
{
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("stop");

    EXPECT_CALL(*main_iDomTools.get(), MPD_stop());
    EXPECT_CALL(*main_iDomTools.get(), saveState_iDom(false));

    auto ret = test_command_mpd->execute(test_v,&test_my_data);
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


    EXPECT_CALL(*main_iDomTools.get(), MPD_play(testing::_));
    EXPECT_CALL(*main_iDomTools.get(), saveState_iDom(false)).Times(2);

    test_command_mpd->execute(test_v,&test_my_data);
    test_v.clear();
    test_v.push_back("mpd");
    test_v.push_back("start");
    test_v.push_back("3");


    EXPECT_CALL(*main_iDomTools.get(), MPD_play(testing::_, 3));
    test_command_mpd->execute(test_v,&test_my_data);
}
