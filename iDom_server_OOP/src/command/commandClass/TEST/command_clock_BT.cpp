#include <gtest/gtest.h>

#include "../command_clock.h"


class command_clock_Class_fixture : public testing::Test
{
public:
    command_clock_Class_fixture()
    {
        test_command_clock = std::make_unique <command_clock> ("clock");
    }

protected:
    std::unique_ptr<command_clock> test_command_clock;
    thread_data test_my_data;
    std::vector<std::string> test_v;

};

TEST_F(command_clock_Class_fixture, main)
{
    test_v.push_back("clock");
    test_v.push_back("1234");
    auto ret = test_command_clock->execute(test_v, &test_my_data);
    EXPECT_STREQ(ret.c_str(),"done");
}

TEST_F(command_clock_Class_fixture, missingParamiter)
{
    test_v.push_back("clock");
    auto ret = test_command_clock->execute(test_v, &test_my_data);
    EXPECT_STREQ(ret.c_str(),"can not set clock");
}
