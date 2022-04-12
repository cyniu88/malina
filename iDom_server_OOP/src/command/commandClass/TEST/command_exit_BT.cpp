#include <gtest/gtest.h>

#include "../commandexit.h"

class command_exit_Class_fixture : public testing::Test
{
public:
    command_exit_Class_fixture():test_command_exit(std::make_unique <commandEXIT>("exit"))
    {
    }


protected:
    std::unique_ptr<commandEXIT> test_command_exit;
    std::vector<std::string> test_v;
    thread_data test_my_data;
};

TEST_F(command_exit_Class_fixture, main)
{
    test_v.push_back("exit");
    auto ret = test_command_exit->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"\nEND.\n");
}
