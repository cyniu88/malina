#include <gtest/gtest.h>

#include "../command_uptime.h"

class command_uptime_Class_fixture : public testing::Test
{
public:
    command_uptime_Class_fixture():test_command_uptime(std::make_unique <command_UPTIME> ("uptime")), test_context()
    {
    }

protected:
    std::unique_ptr<command_UPTIME> test_command_uptime;
    std::vector<std::string> test_v;
    thread_data test_context;
};

TEST_F(command_uptime_Class_fixture, main)
{
    time(&test_context.start);
    test_v.push_back("uptime");
    auto ret = test_command_uptime->execute(test_v,&test_context);
    std::cout << "DATA: " << ret << std::endl;
    EXPECT_STREQ(ret.c_str(),"uptime: \n0 day 0 hours 0 minutes 0 seconds");
}
