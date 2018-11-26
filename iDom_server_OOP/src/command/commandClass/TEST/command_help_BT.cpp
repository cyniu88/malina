#include "../command_help.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_help_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_help_Class_fixture()
    {

    }

protected:
    //command_help* test_command_help = NULL;
    std::unique_ptr<command_help> test_command_help;

    std::vector<std::string> test_v= {"433MHz"};
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
      //  test_command_help = new command_help("help");
        test_command_help =  std::make_unique < command_help> ("help");
    }

    void TearDown() final
    {
      //  delete test_command_help;
        iDomTOOLS_ClassTest::TearDown();

    }
};

TEST_F(command_help_Class_fixture, main)
{
    test_v.push_back("help");
     std::cout << test_command_help->execute(test_v,&test_my_data) << std::endl;
}
