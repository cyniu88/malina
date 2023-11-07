#include <gtest/gtest.h>

#include "../commandtest.h"

class command_test_Class_fixture : public testing::Test
{
public:
    command_test_Class_fixture():  test_command_test{std::make_unique <commandTEST> ("test")}, test_context()
    {

    }

protected:
    std::unique_ptr<commandTEST> test_command_test;
    std::vector<std::string> test_v;
    thread_data test_context;
};

TEST_F(command_test_Class_fixture, test)
{
    test_v.push_back("test");
    test_v.push_back("test");
    auto ret = test_command_test->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(),"test - for test");
}

TEST_F(command_test_Class_fixture, throw_test)
{
    test_v.push_back("test");
    test_v.push_back("throw");
    EXPECT_THROW(test_command_test->execute(test_v,&test_context), int);
}

TEST_F(command_test_Class_fixture, unknownParam)
{
    test_v.push_back("test");
    test_v.push_back("fake");
    auto ret = test_command_test->execute(test_v,&test_context);
    EXPECT_EQ(ret.size(),59);
}
