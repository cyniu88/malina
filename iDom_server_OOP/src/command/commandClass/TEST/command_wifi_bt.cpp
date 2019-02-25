#include "../command_wifi.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_wifi_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_wifi_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_wifi> test_command_wifi;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_wifi = std::make_unique <command_wifi> ("wifi");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
};

TEST_F(command_wifi_Class_fixture, main)
{
    time(&test_my_data.start);
    test_v.push_back("wifi");
    auto ret = test_command_wifi->execute(test_v,&test_my_data);
    std::cout << "DATA: " << ret << std::endl;
    EXPECT_STREQ(ret.c_str(),"done");
}
