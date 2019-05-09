#include <gtest/gtest.h>

#include "../command_idom.h"
#include "../../../functions/functions.h"
#include "../../../RADIO_433_eq/radio_433_eq.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"
#include "../../../thread_functions/iDom_thread.h"

class commandiDom_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    commandiDom_Class_fixture()
    {
        this->test_command_iDom = std::nullptr_t();
        this->test_RFLink = std::nullptr_t();
    }

protected:
    std::vector<std::string> test_v= {"iDom"};
    RFLinkHandler* test_RFLink;
    blockQueue test_q;
    command_iDom* test_command_iDom;

    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();

        test_q._clearAll();

        test_RFLink = new RFLinkHandler(&test_my_data);
        test_command_iDom = new command_iDom("iDom");
        test_my_data.main_RFLink = test_RFLink;
        std::cout << "commandiDom_Class_fixture SetUp" << std::endl;
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
        delete test_RFLink;
        delete test_command_iDom;
        std::cout << "commandiDom_Class_fixture TearDown" << std::endl;
    }
};

TEST_F(commandiDom_Class_fixture, getName)
{
    EXPECT_THAT(test_command_iDom->getCommandName(),testing::HasSubstr("iDom"));
}

TEST_F(commandiDom_Class_fixture, help)
{
    std::string helpStr = test_command_iDom->help();
    EXPECT_THAT(helpStr,testing::HasSubstr("iDom"));
    std::cout << "test " << helpStr.size() << std::endl;
    EXPECT_EQ(helpStr.size(),1232);
}

TEST_F(commandiDom_Class_fixture, less_param)
{
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    EXPECT_THAT(retStr,testing::HasSubstr("need parameter!"));
}

TEST_F(commandiDom_Class_fixture, unknonw_para)
{
    test_v.push_back("fake");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    EXPECT_THAT(retStr,testing::HasSubstr("iDom - unknown parameter:"));
}

TEST_F(commandiDom_Class_fixture, speakers)
{
    test_my_data.main_iDomTools->unlockHome();
    //////////////// fake command
    test_v.push_back("speakers");
    test_v.push_back("fake");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    EXPECT_THAT(retStr,testing::HasSubstr("unknow speakers action: fake"));
    ///////////////// ON
    test_my_data.main_iDomStatus->setObjectState("speakers",STATE::UNDEFINE);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("speakers");
    test_v.push_back("ON");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    EXPECT_THAT(retStr,testing::HasSubstr("speakers ON"));
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::ON);
    ///////////////// OFF
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("speakers");
    test_v.push_back("OFF");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    EXPECT_THAT(retStr,testing::HasSubstr("speakers OFF"));
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
}

TEST_F(commandiDom_Class_fixture, sunset_sunrise)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("sunset");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("Sunset time:"));

    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("sunrise");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("Sunrise time:"));
}

TEST_F(commandiDom_Class_fixture, day_lenght)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("day");
    test_v.push_back("lenght");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("Day Lenght :"));
}

TEST_F(commandiDom_Class_fixture, sun)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("sun");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("Day Lenght :"));
}

TEST_F(commandiDom_Class_fixture, sysinfo)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("sysinfo");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("free RAM"));
}

TEST_F(commandiDom_Class_fixture, temperature)
{
    TEST_DATA::return_send_to_arduino = "-12:22";
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("temperature");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("-12:22"));

    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("temperature");
    test_v.push_back("stats");
    test_v.push_back("insideee");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("not found!"));
}

TEST_F(commandiDom_Class_fixture, text)
{
    TEST_DATA::return_send_to_arduino = "-12:22";
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("text");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("Godzina"));
}

TEST_F(commandiDom_Class_fixture, lock_unlock)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("lock");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_EQ(test_my_data.idom_all_state.houseState, STATE::LOCK);

    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("unlock");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_EQ(test_my_data.idom_all_state.houseState, STATE::UNLOCK);
}

TEST_F(commandiDom_Class_fixture, t_230V)
{
    test_my_data.main_iDomTools->unlockHome();
    //////////////// fake command
    test_v.push_back("230V");
    test_v.push_back("fake");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    EXPECT_THAT(retStr,testing::HasSubstr("wrong paramiter"));
    ///////////////// ON
    test_my_data.main_iDomStatus->setObjectState("printer",STATE::UNDEFINE);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"), STATE::UNDEFINE);
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("230V");
    test_v.push_back("ON");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("230V ON"));
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"), STATE::ON);
    ///////////////// OFF
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("230V");
    test_v.push_back("OFF");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("230V OFF"));
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"), STATE::OFF);
}

TEST_F(commandiDom_Class_fixture, smog)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("smog");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr(" mg/m^3"));
}

TEST_F(commandiDom_Class_fixture, say)
{
    Clock::setTime_forBT_usage(23,23);
    TEST_DATA::return_send_to_arduino = "-3:6";
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("say");
    test_v.push_back("dummy");
    test_v.push_back("dummy");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    //EXPECT_THAT(retStr,testing::HasSubstr("sad"));
}

TEST_F(commandiDom_Class_fixture, wifi)
{
    TEST_DATA::return_httpPost = "ok";
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("wifi");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("ok"));
}

TEST_F(commandiDom_Class_fixture, lightning)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("lightning");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("bool: "));
}

TEST_F(commandiDom_Class_fixture, camera)
{
    test_my_data.server_settings->_camera.cameraLedOFF = "cameraOFF";
    test_my_data.server_settings->_camera.cameraLedON = "cameraON";

    Clock::setTime_forBT_usage(23,23);
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("camera");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_STREQ(retStr.c_str(),"not enough parameters");
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("cameraLED"),STATE::UNKNOWN);
    ////////////////////////////////////////// ON
    TEST_DATA::return_httpPost = "ok.\n";
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("camera");
    test_v.push_back("LED");

    test_v.push_back("ON");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("cameraLED"),STATE::ON);
    EXPECT_STREQ(retStr.c_str(),"led DONE");
    ////////////////////////////////////////// OFF
    TEST_DATA::return_httpPost = "ok.\n";
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("camera");
    test_v.push_back("LED");

    test_v.push_back("OFF");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("cameraLED"),STATE::OFF);
    EXPECT_STREQ(retStr.c_str(),"led DONE");
}

TEST_F(commandiDom_Class_fixture, LED)
{
    test_my_data.main_iDomTools->unlockHome();

    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("LED");
    test_v.push_back("33");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("need more parameter from-to-R-G-B"));

    ///////////////////////////////////// OFF
    TEST_DATA::return_send_to_arduino = "led OFF";

    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("LED");
    test_v.push_back("OFF");
    retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("led OFF"));
}

TEST_F(commandiDom_Class_fixture, kodi)
{
    test_my_data.main_iDomStatus->setObjectState("KODI",STATE::ACTIVE);
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("KODI");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);

    std::cout << "retString: " << retStr << std::endl;
    EXPECT_STREQ(retStr.c_str(),"kodi already run");

    test_my_data.main_iDomStatus->setObjectState("KODI",STATE::DEACTIVE);
    test_my_data.main_iDomStatus->setObjectState("music",STATE::PLAY);
    test_my_data.main_iDomStatus->setObjectState("speakers",STATE::ON);

    std::array<Thread_array_struc,iDomConst::MAX_CONNECTION> test_ThreadArrayStruc;

    for (std::size_t i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
        test_ThreadArrayStruc.at(i).thread_socket = 0;
    test_ThreadArrayStruc.at(3).thread_socket = 0;
    test_ThreadArrayStruc.at(3).thread_ID = std::this_thread::get_id();
    test_my_data.main_THREAD_arr = &test_ThreadArrayStruc;

    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("KODI");
    retStr = test_command_iDom->execute(test_v, &test_my_data);

    //sleep(1);
    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);

    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("STARTED"));

    for (std::size_t i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
        test_ThreadArrayStruc[i].thread_socket = 1;

    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("KODI");
    retStr = test_command_iDom->execute(test_v, &test_my_data);

    std::cout << "retString: " << retStr << std::endl;
    EXPECT_STREQ(retStr.c_str(),"not free space to new thread");
}

TEST_F(commandiDom_Class_fixture, health_alarm)
{
    test_v.clear();
    test_v.push_back("iDom");
    test_v.push_back("health");
    std::string retStr = test_command_iDom->execute(test_v, &test_my_data);
    std::cout << "retString: " << retStr << std::endl;
    EXPECT_THAT(retStr,testing::HasSubstr("no alarms!"));
}
