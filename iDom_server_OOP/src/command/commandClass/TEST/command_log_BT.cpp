#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_log.h"

class command_log_Class_fixture : public testing::Test
{
public:
    command_log_Class_fixture():test_command_log(std::make_unique<command_log>("log")), test_context()
    {
    }

protected:
    std::unique_ptr<command_log> test_command_log;
    std::vector<std::string> test_v;
    thread_data test_context;
};

TEST_F(command_log_Class_fixture, main)
{
    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("INFOoo");
    test_v.push_back("test");
    auto ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));

    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("INFO");
    test_v.push_back("test");
    ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));

    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("DEBUG");
    test_v.push_back("test");
    ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));

    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("WARNING");
    test_v.push_back("test");
    ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));
    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("ERROR");
    test_v.push_back("test");
    ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));
    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("FATAL");
    test_v.push_back("test");
    ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));
    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("CRITICAL");
    test_v.push_back("test");
    ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));
    test_v.clear();
    test_v.push_back("log");
    test_v.push_back("VERBOSE");
    test_v.push_back("test");
    ret = test_command_log->execute(test_v,&test_context);
    EXPECT_THAT(ret, ::testing::HasSubstr("DONE!"));
}
