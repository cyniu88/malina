#include "../command_big.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_big_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_big_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_big> test_command_big;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_big =  std::make_unique <command_big> ("big");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_big_Class_fixture, main)
{
    int s = 55;
    test_v.push_back("big");
    test_v.push_back(std::to_string(s));
    auto ret = test_command_big->execute(test_v,&test_my_data);
    EXPECT_EQ(ret.size(),s);
}
