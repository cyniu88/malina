#include "../command_ok.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_ok_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_ok_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_ok> test_command_ok;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_ok =  std::make_unique <command_ok> ("ok");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_ok_Class_fixture, main)
{
    test_v.push_back("ok");
    auto ret = test_command_ok->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"\nEND\n");
}
