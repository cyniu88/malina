#include "../command_sleep.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_sleep_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_sleep_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_sleep> test_command_sleep;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_sleep = std::make_unique <command_sleep> ("sleep");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_sleep_Class_fixture, sleep)
{
    test_my_data.sleeper = 88;

    test_v.push_back("sleep");
    auto ret = test_command_sleep->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "sleep set to: 88");
}

TEST_F(command_sleep_Class_fixture, sleepFakeParam)
{
    test_my_data.sleeper = 88;

    test_v.push_back("sleep");
    test_v.push_back("fake");
    auto ret = test_command_sleep->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "internal error");
}

TEST_F(command_sleep_Class_fixture, sleepFakeParam2)
{
    test_my_data.sleeper = 88;

    test_v.push_back("sleep");
    test_v.push_back("fake");
    test_v.push_back("fake2");
    auto ret = test_command_sleep->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "wrong parametr fake");
}

TEST_F(command_sleep_Class_fixture, sleepSetNoIntiger)
{
    test_my_data.sleeper = 88;

    test_v.push_back("sleep");
    test_v.push_back("set");
    test_v.push_back("fake2");
    auto ret = test_command_sleep->execute(test_v,&test_my_data);
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

    test_my_data.main_THREAD_arr = &threadArray;

    test_v.push_back("sleep");
    test_v.push_back("set");
    test_v.push_back("2");
    auto ret = test_command_sleep->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "DONE - Sleep MPD STARTED");
    sleep(1);
}
