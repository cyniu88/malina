#include "../command_clock.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_clock_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_clock_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_clock> test_command_clock;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_clock = std::make_unique <command_clock> ("clock");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_clock_Class_fixture, main)
{
    test_v.push_back("clock");
    test_v.push_back("1234");
    auto ret = test_command_clock->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"can not set clock");
}

TEST_F(command_clock_Class_fixture, missingParamiter)
{
    test_v.push_back("clock");
    auto ret = test_command_clock->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"can not set clock");
}
