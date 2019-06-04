#include "../command_gateway.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_gateway_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_gateway_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_gateway> test_command_gateway;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_gateway = std::make_unique <command_gateway> ("gateway");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_gateway_Class_fixture, fan)
{
    test_v.push_back("bgateway");
    test_v.push_back("fan");
    auto ret = test_command_gateway->execute(test_v,&test_my_data);
    EXPECT_THAT(ret,testing::HasSubstr("fan on"));
}
