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

TEST_F(command433MHz_Class_fixture, deleteSwitch)
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
TEST_F(command433MHz_Class_fixture, deleteDeletedSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),4);
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

TEST_F(command433MHz_Class_fixture, addSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),4);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("Aaa");
    test_v.push_back("1234");
    test_v.push_back("SWITCH");
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    test_v.push_back("sunrise_A");
    test_v.push_back("sunset_A");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
}

TEST_F(command433MHz_Class_fixture, missingParamiter_addSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("Abc");
    test_v.push_back("1234");
    test_v.push_back("SWITCH");
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_STREQ(result.substr(0, 15).c_str(), "wrong paramiter")<< "nie ma bledu";
    v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
}
\
TEST_F(command433MHz_Class_fixture, add_wrongType_addSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("Abc");
    test_v.push_back("1234");
    test_v.push_back("FAKE"); //here is wrong type
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    test_v.push_back("sunrise_A");
    test_v.push_back("sunset_A");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_STREQ(result.substr(0, 10).c_str(), "wrong type");
    std::cout << "wynik testu: " << result << std::endl;
    v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
}

TEST_F(command433MHz_Class_fixture, add_wrongID_addSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("Abc");
    test_v.push_back("dummy"); //here is wrong ID
    test_v.push_back("SWITCH");
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    test_v.push_back("sunrise_A");
    test_v.push_back("sunset_A");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    std::cout << "wynik testu: " << result << std::endl;
    EXPECT_STREQ(result.substr(0, 8).c_str(), "wrong ID");
    v = test_rec.getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
}
