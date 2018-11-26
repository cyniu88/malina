#include "../command_hello.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_hello_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_hello_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_hello> test_command_hello;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_hello =  std::make_unique <command_hello> ("hello");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_hello_Class_fixture, main)
{
    test_v.push_back("hello");
    auto ret = test_command_hello->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"\nHI You User!\n");
}
