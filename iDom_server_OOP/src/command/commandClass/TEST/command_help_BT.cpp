#include "../command_help.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_help_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_help_Class_fixture()
    {

    }

protected:
    std::vector<std::string> test_v;

    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_help_Class_fixture, all)
{
    auto chr = std::make_unique<commandHandlerRoot>(&test_my_data);
    test_v.push_back("help");
    auto size = chr->run(test_v,&test_my_data).size();
    EXPECT_EQ(size, 5300);
}

TEST_F(command_help_Class_fixture, one)
{
    auto chr = std::make_unique<commandHandlerRoot>(&test_my_data);
    test_v.push_back("help");
    test_v.push_back("ok");
    auto ret = chr->run(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "ok - confirmation msg server response: END \n");
}

TEST_F(command_help_Class_fixture, nonExistingCommand)
{
    auto chr = std::make_unique<commandHandlerRoot>(&test_my_data);
    test_v.push_back("help");
    test_v.push_back("okdoki");
    auto ret = chr->run(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "unknown command: okdoki help note not found");
}
