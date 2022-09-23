#include<gtest/gtest.h>
#include<gmock/gmock.h>

#include "../command_shed.h"

class command_shed_Class_fixture : public testing::Test
{
public:
    command_shed_Class_fixture():test_command_shed(std::make_unique <command_shed>("shed"))
    {
        test_my_data.m_keyHandler = std::make_unique<iDomKEY_ACCESS>("");
    }

protected:
    std::unique_ptr<command_shed> test_command_shed;

    std::vector<std::string> test_v;
    thread_data test_my_data;
};

TEST_F(command_shed_Class_fixture, wrongParameter)
{
    test_v.push_back("shed");
    test_v.push_back("t");
    auto ret = test_command_shed->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "command shed - wrong paramiter");
}

TEST_F(command_shed_Class_fixture, put_get_json)
{
    std::string msg = R"({"temperatura":21.37999916,"ciśnienie":992.303772,"wilgotność":51.86914063,"millis":119626,"bateria":3.900585938})";
    test_v.push_back("shed");
    test_v.push_back("put");
    test_v.push_back(msg);
    auto ret = test_command_shed->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "DONE");

    test_v.clear();
    test_v.push_back("shed");
    test_v.push_back("show");
    ret = test_command_shed->execute(test_v,&test_my_data);
    std::cout << ret << std::endl;
}
