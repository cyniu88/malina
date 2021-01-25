#include "../command_ip.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_ip_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_ip_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_ip> test_command_ip;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_ip = std::make_unique <command_ip> ("ip");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_ip_Class_fixture, main)
{
    test_my_data.server_settings->_server.SERVER_IP = "100.001.1.0";
    test_v.push_back("ip");
    auto ret = test_command_ip->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"iDom server IP: 100.001.1.0");
}
