#include "../command_buderus.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_buderus_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_buderus_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_buderus> test_command_buderus;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_buderus = std::make_unique <command_buderus> ("buderus");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_buderus_Class_fixture, tapWaterActiv_Deactiv)
{

    test_v.push_back("buderus");
    auto ret = test_command_buderus->execute(test_v,&test_my_data);
    std::cout << "wynik: " << ret << std::endl;
}
