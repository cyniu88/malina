#include <gtest/gtest.h>

#include "../command_hello.h"

class command_hello_Class_fixture : public testing::Test
{
public:
    command_hello_Class_fixture():test_command_hello(std::make_unique <command_hello>("hello")), test_context()
    {
    }

protected:
    std::unique_ptr<command_hello> test_command_hello;
    std::vector<std::string> test_v;
    thread_data test_context;
};

TEST_F(command_hello_Class_fixture, main)
{
    test_v.push_back("hello");
    auto ret = test_command_hello->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(),"\nHI You User!\n");
}
