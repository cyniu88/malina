#include "../command_put.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_put_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_put_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_put> test_command_put;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_put = std::make_unique <command_put> ("put");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_put_Class_fixture, main)
{
    TEST_DATA::return_send_to_arduino = "-2:3";

    test_v.push_back("put");
    test_v.push_back("temperature");
    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "DONE");
}
