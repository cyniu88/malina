#include <gtest/gtest.h>

#include "../command_help.h"
#include "../../commandhandlerroot.h"
#include "../../../RADIO_433_eq/radio_433_eq.h"

class command_help_Class_fixture : public testing::Test
{
public:
    command_help_Class_fixture()
    {
        auto test_rec = std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data);
        test_my_data.main_REC = test_rec;
    }

protected:
    std::vector<std::string> test_v;
    thread_data test_my_data;
};

TEST_F(command_help_Class_fixture, all)
{
    auto chr = std::make_unique<commandHandlerRoot>(&test_my_data);
    test_v.push_back("help");
    auto size = chr->run(test_v,&test_my_data).size();
    EXPECT_EQ(size, 5428);
}

TEST_F(command_help_Class_fixture, one)
{
    auto chr = std::make_unique<commandHandlerRoot>(&test_my_data);
    test_v.push_back("help");
    test_v.push_back("ok");
    auto ret = chr->run(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "ok - confirmation msg server response: END \n");
}

TEST_F(command_help_Class_fixture, nonExistingCommand)
{
    auto chr = std::make_unique<commandHandlerRoot>(&test_my_data);
    test_v.push_back("help");
    test_v.push_back("okdoki");
    auto ret = chr->run(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "unknown command: okdoki help note not found");
}
