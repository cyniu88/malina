#include "../commandexit.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_exit_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_exit_Class_fixture()
    {

    }

protected:
    std::unique_ptr<commandEXIT> test_command_exit;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_exit = std::make_unique <commandEXIT> ("exit");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_exit_Class_fixture, main)
{
    test_v.push_back("exit");
    auto ret = test_command_exit->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"\nEND.\n");
}
