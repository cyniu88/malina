#include <gtest/gtest.h>

#include "../command_put.h"
#include "test_data.h"
#include "../../iDom_server_OOP/src/iDomTools/mock/iDomToolsMock.h"

class command_put_Class_fixture : public testing::Test
{
public:
    command_put_Class_fixture():
        test_command_put(std::make_unique<command_put>("put")),
        main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.main_iDomTools = main_iDomTools;
    }

protected:
    std::unique_ptr<command_put> test_command_put;
    std::vector<std::string> test_v;
    thread_data test_context;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
};

TEST_F(command_put_Class_fixture, main)
{
    TEST_DATA::return_send_to_arduino = "-2:3";
    test_context.lusina.statHumi.push_back(12);
    test_context.lusina.statTemp.push_back(1.13);
    test_v.push_back("put");
    test_v.push_back("temperature");
    EXPECT_CALL(*main_iDomTools.get(), send_data_to_thingSpeak());
    auto ret = test_command_put->execute(test_v,&test_context);
    EXPECT_STREQ(ret.c_str(), "DONE");
}
