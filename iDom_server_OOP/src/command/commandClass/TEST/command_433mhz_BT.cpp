#include<gtest/gtest.h>
#include<gmock/gmock.h>

#include "../command_433mhz.h"
#include "../../../functions/functions.h"
#include "../../../RADIO_433_eq/radio_433_eq.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"

class command433MHz_Class_fixture : public testing::Test
{
public:
    command433MHz_Class_fixture()
    {
        this->test_command_433MHz = std::nullptr_t();
        this->test_RFLink = std::nullptr_t();
    }

protected:
    thread_data test_my_data;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    std::vector<std::string> test_v= {"433MHz"};
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec;
    RFLinkHandler* test_RFLink;
    blockQueue test_q;
    command_433MHz* test_command_433MHz;
    CONFIG_JSON test_server_settings;

    void SetUp() final
    {

        test_q._clearAll();
        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
        test_server_settings._server.radio433MHzConfigFile = "/mnt/ramdisk/433_eq_conf.json";
        test_rec = std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data);
        test_rec->loadConfig(test_server_settings._server.radio433MHzConfigFile);
        test_my_data.main_REC = test_rec;
        test_server_settings._rflink.RFLinkPort = "test port";
        test_my_data.server_settings = &test_server_settings;
        test_command_433MHz = new command_433MHz("433MHz");
        test_my_data.main_RFLink = std::make_shared<RFLinkHandler>(&test_my_data);
        test_my_data.serverStarted = false;
        main_iDomTools = std::make_shared<iDomToolsMock>();
        test_my_data.main_iDomTools = main_iDomTools;
        std::cout << "command433MHz_Class_fixture SetUp" << std::endl;
    }

    void TearDown() final
    {
        delete test_command_433MHz;
        std::cout << "command433MHz_Class_fixture TearDown" << std::endl;
    }

    void addSwitch(const std::string& name)
    {
        test_v.clear();
        test_v.push_back("433MHz");
        test_v.push_back("add");
        test_v.push_back("SWITCH");
        test_v.push_back(name);
        test_v.push_back("1234");
        test_v.push_back("onCode_A");
        test_v.push_back("ofCode_A");
        test_v.push_back("on15sec_A");
        test_v.push_back("sunrise_A");
        test_v.push_back("sunset_A");
        test_v.push_back("lock_A");
        test_v.push_back("unlock_A");
        std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    }
    void deleteSwitch(const std::string& name)
    {
        test_v.clear();
        test_v.push_back("433MHz");
        test_v.push_back("delete");
        test_v.push_back(name);
        std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    }
};
TEST_F(command433MHz_Class_fixture, getCommandName)
{
    EXPECT_THAT(test_command_433MHz->getCommandName(),testing::HasSubstr("433MHz"));
}

TEST_F(command433MHz_Class_fixture, deleteSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("delete");
    test_v.push_back("A");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),4);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    addSwitch("A");

}
TEST_F(command433MHz_Class_fixture, deleteFakeSwitch)
{
    auto actualSize = test_rec->getSwitchPointerVector().size();
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),actualSize);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("delete");
    test_v.push_back("fake");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_THAT(result, testing::HasSubstr("not exist"));
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),actualSize);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
}
TEST_F(command433MHz_Class_fixture, addButton)
{
    auto actualSize = test_rec->getButtonPointerVector().size();
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getButtonPointerVector();
    EXPECT_EQ(v.size(),actualSize);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("BUTTON");
    test_v.push_back("glowny");
    test_v.push_back("4321");
    test_v.push_back("onCode");
    test_v.push_back("offCode");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    v = test_rec->getButtonPointerVector();
    EXPECT_EQ(v.size(),actualSize + 1);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    deleteSwitch("glowny");
}

TEST_F(command433MHz_Class_fixture, addSwitch)
{
    auto actualSize = test_rec->getSwitchPointerVector().size();
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),actualSize);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("SWITCH");
    test_v.push_back("Aaa");
    test_v.push_back("1234");
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    test_v.push_back("sunrise_A");
    test_v.push_back("sunset_A");
    test_v.push_back("lock_A");
    test_v.push_back("unlock_A");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),actualSize + 1);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    deleteSwitch("Aaa");
}

TEST_F(command433MHz_Class_fixture, addExistingWeather)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getWeather_StationPtrVector();
    EXPECT_EQ(v.size(),1);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("WEATHER");
    test_v.push_back("first");
    test_v.push_back("1234");

    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    v = test_rec->getWeather_StationPtrVector();
    EXPECT_EQ(v.size(),1);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
}

TEST_F(command433MHz_Class_fixture, missingParamiter_addSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("SWITCH");
    test_v.push_back("Abc");
    test_v.push_back("1234");
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_THAT(result, testing::HasSubstr("mising paramiter")) << "nie ma bledu";
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
}
\
TEST_F(command433MHz_Class_fixture, add_wrongType_addSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("FAKE"); //here is wrong type
    test_v.push_back("Abc");
    test_v.push_back("1234");
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    test_v.push_back("sunrise_A");
    test_v.push_back("sunset_A");
    test_v.push_back("lock_A");
    test_v.push_back("unlock_A");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_THAT(result, testing::HasSubstr("wrong type"));
    std::cout << "wynik testu: " << result << std::endl;
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
}

TEST_F(command433MHz_Class_fixture, add_wrongID_addSwitch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("add");
    test_v.push_back("SWITCH");
    test_v.push_back("Abc");
    test_v.push_back("dummy"); //here is wrong ID
    test_v.push_back("onCode_A");
    test_v.push_back("ofCode_A");
    test_v.push_back("on15sec_A");
    test_v.push_back("sunrise_A");
    test_v.push_back("sunset_A");
    test_v.push_back("lock_A");
    test_v.push_back("unlock_A");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    std::cout << "wynik testu: " << result << std::endl;
    EXPECT_THAT(result, testing::HasSubstr("wrong ID"));
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
}

TEST_F(command433MHz_Class_fixture, show_config)
{
    test_v.push_back("show");
    test_v.push_back("config");
    auto retStr = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_GT(retStr.size(), 2210);
}

TEST_F(command433MHz_Class_fixture, show_switch)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) << std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("switch");

    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    std::cout << "wynik testu: " << result << std::endl;
    EXPECT_THAT(result, testing::HasSubstr("UNDEFINE"));
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
}

TEST_F(command433MHz_Class_fixture, show_aether)
{
    test_my_data.main_RFLink->m_rflinkMAP["kk"].msg = "astro";
    test_my_data.main_RFLink->m_rflinkMAP["kk"].m_counter = 99;
    test_my_data.main_RFLink->m_rflinkMAP["jj"].msg = "lock";
    test_my_data.main_RFLink->m_rflinkMAP["jj"].m_counter = 155;
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("show");
    test_v.push_back("aether");

    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    std::cout << "wynik testu: " << result << std::endl;
    EXPECT_THAT(result, testing::HasSubstr("astro"));
    EXPECT_THAT(result, testing::HasSubstr("lock"));
    EXPECT_THAT(result, testing::HasSubstr("99"));
    EXPECT_THAT(result, testing::HasSubstr("155"));
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
}

TEST_F(command433MHz_Class_fixture, switchRF433)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    /////////////////////////////// ON
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("B");
    test_v.push_back("ON");
    test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::ON);
    ////////////////////////////// OFF
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("B");
    test_v.push_back("OFF");
    test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::OFF);
    ////////////////////////////// change (ON)
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("B");
    test_v.push_back("change");
    test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::ON);
    ////////////////////////////// change (OFF)
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("B");
    test_v.push_back("change");
    test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::OFF);
    ////////////////////////////// 15s
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("B");
    test_v.push_back("15s");
    test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::TEMPORARY);
    ////////////////////////////// unknown paramiter
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("B");
    test_v.push_back("fake");
    std::string result = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_EQ(test_my_data.main_REC->getEqPointer("B")->getState(), STATE::TEMPORARY);
    EXPECT_THAT(result, testing::HasSubstr("fake"));
    ///////////////////////////// fake switch
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("B_fake");
    test_v.push_back("fake");

    result = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_THAT(result, testing::HasSubstr("not found"));
    v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
}


TEST_F(command433MHz_Class_fixture, sendRF433)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("send");
    test_v.push_back("fake");

    std::string retStr = test_command_433MHz->execute(test_v,&test_my_data);
    EXPECT_THAT(retStr, testing::HasSubstr("sended"));
}

TEST_F(command433MHz_Class_fixture, fakeSwitchON)
{
    test_v.push_back("show");
    test_v.push_back("all");
    std::cout << test_command_433MHz->execute(test_v,&test_my_data) <<std::endl;
    auto v = test_rec->getSwitchPointerVector();
    EXPECT_EQ(v.size(),5);
    test_v.clear();
    test_v.push_back("433MHz");
    test_v.push_back("switch");
    test_v.push_back("ALARM-fake");
    test_v.push_back("ON");
    EXPECT_CALL(*main_iDomTools.get(),saveState_iDom(false));
    std::string retStr = test_command_433MHz->execute(test_v, &test_my_data);
    EXPECT_THAT(retStr, testing::HasSubstr(" not found ALARM-fake"));
}
