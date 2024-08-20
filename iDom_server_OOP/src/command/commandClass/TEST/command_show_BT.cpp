#include<gtest/gtest.h>
#include<gmock/gmock.h>

#include "../command_show.h"

class command_show_Class_fixture : public testing::Test
{
public:
    command_show_Class_fixture():test_command_show(std::make_unique <command_show>("show"))
    {
        test_context.m_keyHandler = std::make_unique<iDomKEY_ACCESS>("");
    }

protected:
    std::unique_ptr<command_show> test_command_show;

    std::vector<std::string> test_v;
    thread_context test_context;
};

TEST_F(command_show_Class_fixture, wrongParameter)
{
    test_v.push_back("show");
    test_v.push_back("t");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "wrong parameter: t");
}

TEST_F(command_show_Class_fixture, unknownParameter)
{
    test_v.push_back("show");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "show what?");
}

TEST_F(command_show_Class_fixture, showThreadAll)
{
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> threadArray;

    threadArray[1].thread_ID = std::this_thread::get_id();
    threadArray[1].thread_name = "thread1 test1";
    threadArray[1].thread_socket = 1;
    threadArray[2].thread_ID = std::this_thread::get_id();
    threadArray[2].thread_name = "thread1 test2";
    threadArray[2].thread_socket = 2;

    test_context.main_THREAD_arr = &threadArray;

    test_v.push_back("show");
    test_v.push_back("thread");
    test_v.push_back("all");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_THAT(ret, testing::HasSubstr("socket: 2"));
}

TEST_F(command_show_Class_fixture, showThread)
{
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> threadArray;

    threadArray[1].thread_ID = std::this_thread::get_id();
    threadArray[1].thread_name = "thread1 test1";
    threadArray[1].thread_socket = 1;
    threadArray[2].thread_ID = std::this_thread::get_id();
    threadArray[2].thread_name = "thread1 test2";
    threadArray[2].thread_socket = 2;

    test_context.main_THREAD_arr = &threadArray;

    test_v.push_back("show");
    test_v.push_back("thread");
    test_v.push_back("2");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_THAT(ret, testing::HasSubstr("socket: 2"));
}

TEST_F(command_show_Class_fixture, showThreadNoId)
{
    test_v.push_back("show");
    test_v.push_back("thread");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "No ID");
}

TEST_F(command_show_Class_fixture, showLog)
{
    test_v.push_back("show");
    test_v.push_back("log");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_THAT(ret, testing::HasSubstr("VERBOSE"));
}

TEST_F(command_show_Class_fixture, showLogNoInfo)
{
    test_v.push_back("show");
    test_v.push_back("log");
    test_v.push_back("no");
    test_v.push_back("INFO");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_THAT(ret, testing::HasSubstr("VERBOSE"));
    EXPECT_THAT(ret, testing::Not(testing::HasSubstr("INFO")));
}

TEST_F(command_show_Class_fixture, showLogInfo)
{
    test_v.push_back("show");
    test_v.push_back("log");
    test_v.push_back("INFO");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_THAT(ret, testing::HasSubstr("INFO"));
}

TEST_F(command_show_Class_fixture, showLogFakeInfo)
{
    test_v.push_back("show");
    test_v.push_back("log");
    test_v.push_back("fake");
    test_v.push_back("INFO");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "do you mean : show log no <string>?");
}

TEST_F(command_show_Class_fixture, showKEY_all)
{
    test_v.push_back("show");
    test_v.push_back("iDom");
    test_v.push_back("key");
    auto ret = test_command_show->execute(test_v,&test_context);
    EXPECT_GT(ret.size(), 3);
}
