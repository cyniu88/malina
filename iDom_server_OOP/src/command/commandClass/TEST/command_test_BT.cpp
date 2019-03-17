#include "../commandtest.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_test_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_test_Class_fixture()
    {

    }

protected:
    std::unique_ptr<commandTEST> test_command_test;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_test = std::make_unique <commandTEST> ("test");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_test_Class_fixture, test)
{
    test_v.push_back("test");
    test_v.push_back("test");
    auto ret = test_command_test->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"test - for test");
}

TEST_F(command_test_Class_fixture, throw)
{
    test_v.push_back("test");
    test_v.push_back("throw");
    EXPECT_THROW(test_command_test->execute(test_v,&test_my_data), int);
}

TEST_F(command_test_Class_fixture, unknownParam)
{
    test_v.push_back("test");
    test_v.push_back("fake");
    auto ret = test_command_test->execute(test_v,&test_my_data);
    EXPECT_EQ(ret.size(),59);
}
