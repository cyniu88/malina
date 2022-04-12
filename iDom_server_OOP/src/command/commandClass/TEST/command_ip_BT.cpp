#include <gtest/gtest.h>

#include "../command_ip.h"

class command_ip_Class_fixture : public testing::Test
{
public:
    command_ip_Class_fixture():test_command_ip(std::make_unique <command_ip>("ip"))
    {
        test_my_data.server_settings = &server_settings;
    }

protected:
    std::unique_ptr<command_ip> test_command_ip;
    std::vector<std::string> test_v;
    thread_data test_my_data;
    CONFIG_JSON server_settings;
};

TEST_F(command_ip_Class_fixture, main)
{
    test_my_data.server_settings->_server.SERVER_IP = "100.001.1.0";
    test_v.push_back("ip");
    auto ret = test_command_ip->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"iDom server IP: 100.001.1.0");
}
