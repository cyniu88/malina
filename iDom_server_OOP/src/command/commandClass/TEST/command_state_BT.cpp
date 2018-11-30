#include "../command_state.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_state_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_state_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_state> test_command_state;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_state = std::make_unique <command_state> ("state");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_state_Class_fixture, stateAll)
{
    test_v.clear();
    test_v.push_back("state");
    test_v.push_back("all");
    auto ret = test_command_state->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "state: KODI=DEACTIVE alarm=DEACTIVE cameraLED=UNKNOWN house=UNDEFINE printer=OFF speakers=OFF ");
}

TEST_F(command_state_Class_fixture, stateGetOne)
{
    test_v.clear();
    test_v.push_back("state");
    test_v.push_back("KODI");
    auto ret = test_command_state->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "DEACTIVE");
}

TEST_F(command_state_Class_fixture, stateMissingParam)
{
    test_v.clear();
    test_v.push_back("state");
    auto ret = test_command_state->execute(test_v,&test_my_data);
    EXPECT_THAT(ret, testing::HasSubstr("need parameter!\n"));
}
