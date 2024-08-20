#include <gtest/gtest.h>

#include "../commandexit.h"

class command_exit_Class_fixture : public testing::Test
{
public:
    command_exit_Class_fixture():test_command_exit(std::make_unique <commandEXIT>("exit")), test_context()
    {
    }


protected:
    std::unique_ptr<commandEXIT> test_command_exit;
    std::vector<std::string> test_v;
    thread_context test_context;
};

TEST_F(command_exit_Class_fixture, main)
{
    test_v.push_back("exit");
    auto ret = test_command_exit->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(),"\nEND.\n");
}
