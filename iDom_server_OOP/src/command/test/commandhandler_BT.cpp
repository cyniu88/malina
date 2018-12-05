#include "../commandhandlerrs232.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

class command_handler_rs232_fixture : public iDomTOOLS_ClassTest
{
public:
    command_handler_rs232_fixture()
    {

    }

protected:
    std::unique_ptr<commandHandlerRS232> test_chRS232;
    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_chRS232 = std::make_unique<commandHandlerRS232>(&test_my_data);
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_handler_rs232_fixture, main)
{
    test_v.push_back("ok");
    auto ret = test_chRS232->run(test_v, &test_my_data);
    EXPECT_STREQ(ret.c_str(), "\nEND\n");
}
