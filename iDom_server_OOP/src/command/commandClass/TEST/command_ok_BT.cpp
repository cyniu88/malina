#include <gtest/gtest.h>

#include "../command_ok.h"

class command_ok_Class_fixture : public testing::Test
{
public:
    command_ok_Class_fixture():test_command_ok(std::make_unique <command_ok> ("ok")), test_context()
    {
    }

protected:
    std::unique_ptr<command_ok> test_command_ok;
    std::vector<std::string> test_v;
    thread_context test_context;
};

TEST_F(command_ok_Class_fixture, main)
{
    test_v.push_back("ok");
    auto ret = test_command_ok->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(),"\nEND\n");
}
