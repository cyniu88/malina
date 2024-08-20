#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../command_cmd.h"

class command_cmd_Class_fixture : public testing::Test
{
public:
    command_cmd_Class_fixture():test_command_cmd(std::make_unique <command_cmd>("cmd")), test_context()
    {
    }

protected:
    std::unique_ptr<command_cmd> test_command_cmd;
    std::vector<std::string> test_v;
    thread_context test_context;
};

TEST_F(command_cmd_Class_fixture, wrongParamiter)
{
    test_v.push_back("cmd");
    test_v.push_back("1234");
    auto ret = test_command_cmd->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(),"error: unknown parameter: 1234");
}
