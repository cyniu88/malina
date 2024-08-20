#include<gtest/gtest.h>

#include "../command_sleep.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"

class command_sleep_Class_fixture : public testing::Test
{
public:
    command_sleep_Class_fixture():
        test_command_sleep(std::make_unique <command_sleep> ("sleep")),
        main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.main_iDomTools = main_iDomTools;
    }

protected:
    std::unique_ptr<command_sleep> test_command_sleep;
    std::vector<std::string> test_v;
    thread_context test_context;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
};

TEST_F(command_sleep_Class_fixture, sleep)
{
    test_context.sleeper = 88;

    test_v.push_back("sleep");
    auto ret = test_command_sleep->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "sleep set to: 88");
}

TEST_F(command_sleep_Class_fixture, sleepFakeParam)
{
    test_context.sleeper = 88;

    test_v.push_back("sleep");
    test_v.push_back("fake");
    auto ret = test_command_sleep->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "internal error");
}

TEST_F(command_sleep_Class_fixture, sleepFakeParam2)
{
    test_context.sleeper = 88;

    test_v.push_back("sleep");
    test_v.push_back("fake");
    test_v.push_back("fake2");
    auto ret = test_command_sleep->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "wrong parametr fake");
}

TEST_F(command_sleep_Class_fixture, sleepSetNoIntiger)
{
    test_context.sleeper = 88;

    test_v.push_back("sleep");
    test_v.push_back("set");
    test_v.push_back("fake2");
    auto ret = test_command_sleep->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "system need intiger > 0 not: fake2");
}

TEST_F(command_sleep_Class_fixture, sleepSet)
{
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> threadArray;

    threadArray[1].thread_ID = std::this_thread::get_id();
    threadArray[1].thread_name = "thread1 test1";
    threadArray[1].thread_socket = 1;
    threadArray[2].thread_ID = std::this_thread::get_id();
    threadArray[2].thread_name = "thread1 test2";
    threadArray[2].thread_socket = 2;

    test_context.main_THREAD_arr = &threadArray;

    test_v.push_back("sleep");
    test_v.push_back("set");
    test_v.push_back("2");
    EXPECT_CALL(*main_iDomTools.get(), ledClear(0, 60));
    EXPECT_CALL(*main_iDomTools.get(), ledOFF());
    EXPECT_CALL(*main_iDomTools.get(), MPD_stop());
    EXPECT_CALL(*main_iDomTools.get(), turnOff433MHzSwitch("listwa"));
    EXPECT_CALL(*main_iDomTools.get(), ledClear(0, 30));
    auto ret = test_command_sleep->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "DONE - Sleep MPD STARTED");
    sleep(1);
}
