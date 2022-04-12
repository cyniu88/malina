#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_state.h"
#include "../../../functions/functions.h"

class command_state_Class_fixture : public testing::Test
{
public:
    command_state_Class_fixture():test_command_state(std::make_unique <command_state>("state"))
    {
        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
        test_my_data.ptr_buderus = std::make_unique<BUDERUS>();
        useful_F::myStaticData = &test_my_data;
    }

protected:
    std::unique_ptr<command_state> test_command_state;
    std::vector<std::string> test_v;
    thread_data test_my_data;
};

TEST_F(command_state_Class_fixture, stateAll)
{
    test_v.clear();
    test_v.push_back("state");
    test_v.push_back("all");
    auto ret = test_command_state->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "state: burnGas=DEACTIVE ");
}

TEST_F(command_state_Class_fixture, stateGetOne)
{
    test_v.clear();
    test_v.push_back("state");
    test_v.push_back("KODI");
    auto ret = test_command_state->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "UNKNOWN KODI");
}

TEST_F(command_state_Class_fixture, stateMissingParam)
{
    test_v.clear();
    test_v.push_back("state");
    auto ret = test_command_state->execute(test_v,&test_my_data);
    EXPECT_THAT(ret, testing::HasSubstr("need parameter!\n"));
}
