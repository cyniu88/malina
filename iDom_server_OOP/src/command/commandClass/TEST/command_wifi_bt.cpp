#include <gtest/gtest.h>

#include "../command_wifi.h"

class command_wifi_Class_fixture : public testing::Test
{
public:
    command_wifi_Class_fixture():test_command_wifi(std::make_unique <command_wifi>("wifi"))
    {
    }

protected:
    std::unique_ptr<command_wifi> test_command_wifi;
    std::vector<std::string> test_v;
    thread_data test_my_data;
};

TEST_F(command_wifi_Class_fixture, main)
{
    time(&test_my_data.start);
    test_v.push_back("wifi");
    auto ret = test_command_wifi->execute(test_v,&test_my_data);
    std::cout << "DATA: " << ret << std::endl;
    EXPECT_STREQ(ret.c_str(),"done");
}
