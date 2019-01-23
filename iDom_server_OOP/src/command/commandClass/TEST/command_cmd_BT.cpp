#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../command_cmd.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_cmd_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_cmd_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_cmd> test_command_cmd;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_cmd = std::make_unique <command_cmd> ("cmd");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_cmd_Class_fixture, main)
{
    test_my_data.server_settings->_server.omxplayerFile = "/mnt/ramdisk/cmd";
    test_v.push_back("cmd");
    auto ret = test_command_cmd->execute(test_v,&test_my_data);
    EXPECT_THAT(ret, ::testing::HasSubstr("fifo file contain:"));
}

TEST_F(command_cmd_Class_fixture, wrongParamiter)
{
    test_v.push_back("cmd");
    test_v.push_back("1234");
    auto ret = test_command_cmd->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"error: unknown parameter: 1234");
}
