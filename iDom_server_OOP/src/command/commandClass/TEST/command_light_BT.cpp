#include "../command_light.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_light_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_light_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_light> test_command_light;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_light = std::make_unique <command_light> ("light");
        test_my_data.main_house_lighting_handler = std::make_shared<house_lighting_handler>(&test_my_data);

    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_light_Class_fixture, main)
{
    test_v.push_back("light");
    test_v.push_back(std::to_string("0;111;46;1;0;2;0"));
    auto ret = test_command_light->execute(test_v,&test_my_data);
    std::cout << "ret: " << ret << std::endl;
}
