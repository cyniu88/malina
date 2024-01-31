#include <gtest/gtest.h>

#include "../command_big.h"

class command_big_Class_fixture : public testing::Test
{
public:
    command_big_Class_fixture() : test_command_big(std::make_unique<command_big>("big")), test_context()
    {
    }

protected:
    std::unique_ptr<command_big> test_command_big;

    std::vector<std::string> test_v;
    thread_data test_context;
};

TEST_F(command_big_Class_fixture, main)
{
    int s = 55;
    test_v.push_back("big");
    test_v.push_back(std::to_string(s));
    auto ret = test_command_big->execute(test_v, &test_context);
    EXPECT_EQ(ret.size(), s);
}
