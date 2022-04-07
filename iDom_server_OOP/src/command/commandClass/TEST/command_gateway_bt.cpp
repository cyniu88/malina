#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_gateway.h"
#include "../../iDom_server_OOP/src/iDomTools/mock/iDomToolsMock.h"

class command_gateway_Class_fixture : public testing::Test
{
public:
    command_gateway_Class_fixture():main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_command_gateway = std::make_unique <command_gateway> ("gateway");
        test_my_data.main_iDomTools = main_iDomTools;
    }

protected:
    std::unique_ptr<command_gateway> test_command_gateway;
    std::vector<std::string> test_v;
    thread_data test_my_data;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
};

TEST_F(command_gateway_Class_fixture, unknownParam)
{
    test_v.push_back("gateway");
    test_v.push_back("fake");
    auto ret = test_command_gateway->execute(test_v,&test_my_data);
    EXPECT_THAT(ret,testing::HasSubstr("unknown"));
}

TEST_F(command_gateway_Class_fixture, fan)
{
    EXPECT_CALL(*main_iDomTools.get(), turnOn433MHzSwitch("fan"));
    test_v.push_back("gateway");
    test_v.push_back("fan");
    auto ret = test_command_gateway->execute(test_v,&test_my_data);
    EXPECT_THAT(ret,testing::HasSubstr("fan on"));
}
