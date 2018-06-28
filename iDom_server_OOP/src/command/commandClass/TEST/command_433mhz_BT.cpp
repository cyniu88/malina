#include "../command_433mhz.h"
#include "../../../functions/functions.h"
#include "../../../RADIO_433_eq/radio_433_eq.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command433MHz_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command433MHz_Class_fixture()  {  }

protected:
    std::vector<std::string> test_v= {"433MHz"};
    RFLinkHandler* test_RFLink;
    blockQueue test_q;
    command_433MHz* test_command_433MHz;

    void SetUp()
    {
        iDomTOOLS_ClassTest::SetUp();

        test_q._clearAll();

        test_RFLink = new RFLinkHandler(&test_my_data);
        test_command_433MHz = new command_433MHz("433MHz");
        test_my_data.main_RFLink = test_RFLink;
        std::cout << "command433MHz_Class_fixture SetUp" << std::endl;
    }

    void TearDown()
    {
        iDomTOOLS_ClassTest::TearDown();
        delete test_RFLink;
        delete test_command_433MHz;
        std::cout << "command433MHz_Class_fixture TearDown" << std::endl;
    }
};

TEST_F(command433MHz_Class_fixture, wrongMSGformat)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("delete");
    test_v.push_back("A");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),4);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    EXPECT_EQ(1,1);
}
