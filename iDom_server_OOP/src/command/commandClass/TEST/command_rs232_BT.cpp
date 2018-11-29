#include "../commandrs232.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_RS232_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_RS232_Class_fixture()
    {

    }

protected:
    std::unique_ptr<commandRS232> test_command_put;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_put = std::make_unique <commandRS232> ("RS232");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_RS232_Class_fixture, wrongParameter)
{
    test_v.push_back("RS232");
    test_v.push_back("test_par");
    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "wrong parameter: test_par");
}

TEST_F(command_RS232_Class_fixture, getWrongParameter)
{
    test_v.push_back("RS232");
    test_v.push_back("get");
    test_v.push_back("fake");
    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "wrong parameter: fake");
}

TEST_F(command_RS232_Class_fixture, getTemperature)
{
    TEST_DATA::return_send_to_arduino = "-2:-21";

    test_v.push_back("RS232");
    test_v.push_back("get");
    test_v.push_back("temperature");
    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_THAT(ret, ::testing::HasSubstr("-2:-21"));
}

TEST_F(command_RS232_Class_fixture, send)
{
    TEST_DATA::return_send_to_arduino = "done";

    test_v.push_back("RS232");
    test_v.push_back("send");
    test_v.push_back("test:33;");
    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "done");
}

TEST_F(command_RS232_Class_fixture, errorMissingPara)
{
    TEST_DATA::return_send_to_arduino = "done";

    test_v.push_back("RS232");
    test_v.push_back("error");
    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "add more parameter to error");
}

TEST_F(command_RS232_Class_fixture, errorTemperatureMissingPara)
{
    TEST_DATA::return_send_to_arduino = "done";

    test_v.push_back("RS232");
    test_v.push_back("error");
    test_v.push_back("Temperature");
    test_v.push_back("error");
    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "default");
}

TEST_F(command_RS232_Class_fixture, msgTestMissing)
{
    TEST_DATA::return_send_to_arduino = "done";

    test_v.push_back("RS232");
    test_v.push_back("error");
    test_v.push_back("test");
    test_v.push_back("msg");

    test_v.push_back("fake msg");

    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "DONE!");
}

TEST_F(command_RS232_Class_fixture, msgTestFake)
{
    TEST_DATA::return_send_to_arduino = "done";

    test_v.push_back("RS232");
    test_v.push_back("error");
    test_v.push_back("test");
    test_v.push_back("fake");;

    auto ret = test_command_put->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "default");
}
