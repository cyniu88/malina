#include "../command_help.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_help_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_help_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_help> test_command_help;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_help =  std::make_unique <command_help> ("help");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_help_Class_fixture, main)
{
    commandHandlerRoot* chr = new commandHandlerRoot(&test_my_data);
    test_v.push_back("help");
    auto size = test_command_help->execute(test_v,&test_my_data).size();
    EXPECT_EQ(size,4376);
    delete chr;
}
