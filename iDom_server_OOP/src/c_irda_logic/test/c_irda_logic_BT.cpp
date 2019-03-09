#include <gtest/gtest.h>
#include "../../iDom_server_OOP.h"
#include "test_data.h"
#include "../../../iDom_server_OOP/src/iDomTools/test/iDomTools_fixture.h"
#include "../c_irda_logic.h"
#include "../../thread_functions/iDom_thread.h"

class c_irda_logic_fixture : public iDomTOOLS_ClassTest
{
public:
    std::unique_ptr<c_irda_logic> test_irda;
    menu_tree* test_menuTree;
    files_tree* test_filesTree;
    std::string test_omxplayerFile = "../config/cmd_test";
    void SetUp()
    {
        iDomTOOLS_ClassTest::SetUp();
        test_irda = std::make_unique<c_irda_logic>(&test_my_data);
        test_menuTree = new menu_tree("../config/MENU/", test_my_data.mainLCD);
        test_my_data.main_MENU = test_menuTree;

        test_filesTree = new files_tree("../config/MOVIE/", test_my_data.mainLCD);
        test_my_data.main_tree = test_filesTree;

        test_my_data.server_settings->_server.omxplayerFile =  test_omxplayerFile;

        std::cout << "c_irda_logic_fixture SetUp()"<<std::endl;
    }

    void TearDown()
    {
        delete test_menuTree;
        delete test_filesTree;
        iDomTOOLS_ClassTest::TearDown();
        std::cout << "c_irda_logic_fixture TearDown()"<<std::endl;
    }
};

TEST_F(c_irda_logic_fixture, kodi)
{
    std::array<Thread_array_struc,iDomConst::MAX_CONNECTION >test_ThreadArrayStruc;

    for (std::size_t i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
        test_ThreadArrayStruc.at(i).thread_socket = 0;
    test_ThreadArrayStruc.at(1).thread_socket = 0;
    test_ThreadArrayStruc.at(1).thread_ID = std::this_thread::get_id();
    test_my_data.main_THREAD_arr = &test_ThreadArrayStruc;

    test_my_data.main_iDomTools->unlockHome();
    test_my_data.main_iDomStatus->setObjectState("music",STATE::PAUSE);
    test_my_data.main_iDomStatus->setObjectState("speakers",STATE::OFF);
    test_irda->_add(PILOT_KEY::KEY_REFRESH);
    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);
}

TEST_F(c_irda_logic_fixture, turnOnOffListwa)
{
    test_my_data.main_iDomTools->unlockHome();
    test_my_data.main_iDomStatus->setObjectState("listwa",STATE::OFF);

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("listwa")->getState(), STATE::UNDEFINE);
    test_irda->_add(PILOT_KEY::KEY_TEXT);

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("listwa")->getState(),
              STATE::ON) << "wrong state: "
                         << stateToString(test_my_data.main_REC->getEqPointer("listwa")->getState());
    test_irda->_add(PILOT_KEY::KEY_TEXT);

    EXPECT_EQ(test_my_data.main_REC->getEqPointer("listwa")->getState(),
              STATE::OFF) << "wrong state: "
                          << stateToString(test_my_data.main_REC->getEqPointer("listwa")->getState());
}

TEST_F(c_irda_logic_fixture, turnOnOffPrinter)
{
    setReturnPinState(0);
    test_my_data.main_iDomTools->unlockHome();
    test_my_data.main_iDomStatus->setObjectState("printer", STATE::OFF);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"),
              STATE::OFF);

    test_irda->mainPilotHandler(PILOT_KEY::KEY_FAVORITES);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("printer"),
              STATE::ON);
    std::cout <<"event: " << test_my_data.myEventHandler.run("230V")->getEvent() <<std::endl;
}

TEST_F(c_irda_logic_fixture, irdaMPD)
{
    blockQueue test_Q;
    test_Q._clearAll();

    test_irda->_add(PILOT_KEY::KEY_POWER);
    EXPECT_EQ(test_Q._get(), MPD_COMMAND::STOP);

    test_irda->_add(PILOT_KEY::KEY_TV);
    EXPECT_EQ(test_Q._get(), MPD_COMMAND::PLAY);

    test_irda->_add(PILOT_KEY::KEY_VOLUMEDOWN);
    EXPECT_EQ(test_Q._get(), MPD_COMMAND::VOLDOWN);

    test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    EXPECT_EQ(test_Q._get(), MPD_COMMAND::VOLUP);

    test_irda->_add(PILOT_KEY::KEY_AUDIO);
    EXPECT_EQ(test_Q._get(), MPD_COMMAND::PAUSE);

    test_irda->_add(PILOT_KEY::KEY_UP);
    EXPECT_EQ(test_Q._get(), MPD_COMMAND::NEXT);

    test_irda->_add(PILOT_KEY::KEY_DOWN);
    EXPECT_EQ(test_Q._get(), MPD_COMMAND::PREV);

    test_irda->_add(PILOT_KEY::KEY_2); //default
    EXPECT_EQ(test_Q._size(), 0);
}

TEST_F(c_irda_logic_fixture, sleeper_Logic_EXIT)
{
    test_my_data.sleeper = 0;
    EXPECT_EQ(test_my_data.sleeper, 0);
    test_irda->_add(PILOT_KEY::KEY_MENU);
    do {
        test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    } while(TEST_DATA::LCD_print != "5.SLEEPer");
    do {
        test_irda->_add(PILOT_KEY::KEY_VOLUMEDOWN);
    } while(TEST_DATA::LCD_print != "5.SLEEPer");
    test_irda->_add(PILOT_KEY::KEY_OK);
    test_irda->_add(PILOT_KEY::KEY_UP);
    EXPECT_EQ(test_my_data.sleeper, 1);
    test_irda->_add(PILOT_KEY::KEY_UP);
    EXPECT_EQ(test_my_data.sleeper, 2);
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    EXPECT_EQ(test_my_data.sleeper, 0);
}

TEST_F(c_irda_logic_fixture, sleeper_Logic_OK)
{
    std::cout << test_my_data.main_REC->listAllName() << std::endl;

    std::array<Thread_array_struc,iDomConst::MAX_CONNECTION> test_ThreadArrayStruc;

    for (int i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
        test_ThreadArrayStruc.at(i).thread_socket = i+1;
    test_ThreadArrayStruc.at(3).thread_socket = 0;
    test_ThreadArrayStruc.at(3).thread_ID = std::this_thread::get_id();
    test_my_data.main_THREAD_arr = &test_ThreadArrayStruc;
    test_my_data.sleeper = 0;
    EXPECT_EQ(test_my_data.sleeper, 0);
    test_irda->_add(PILOT_KEY::KEY_MENU);
    do {
        test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    } while(TEST_DATA::LCD_print != "5.SLEEPer");
    test_irda->_add(PILOT_KEY::KEY_OK);
    test_irda->_add(PILOT_KEY::KEY_UP);
    EXPECT_EQ(test_my_data.sleeper, 1);
    test_irda->_add(PILOT_KEY::KEY_UP);
    EXPECT_EQ(test_my_data.sleeper, 2);
    test_irda->_add(PILOT_KEY::KEY_CHANNELUP);
    EXPECT_EQ(test_my_data.sleeper, 12);
    test_irda->_add(PILOT_KEY::KEY_CHANNELDOWN);
    EXPECT_EQ(test_my_data.sleeper, 2);
    test_irda->_add(PILOT_KEY::KEY_DOWN);
    EXPECT_EQ(test_my_data.sleeper, 1);
    test_irda->_add(PILOT_KEY::KEY_OK);
    sleep(2);
    EXPECT_EQ(test_my_data.sleeper, 0);
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    EXPECT_EQ(test_my_data.sleeper, 0);\

    //////////////default
    test_irda->who = PILOT_STATE::SLEEPER;
    test_irda->_add(PILOT_KEY::KEY_0); //default
}

TEST_F(c_irda_logic_fixture, LED_ON_OFF)
{
    EXPECT_EQ( test_my_data.myEventHandler.run("LED")->getLast1minNumberEvent(),0);
    for (int i = 1 ; i < test_my_data.ptr_pilot_led->colorLED.size()+2; ++i)
    {
        test_irda->_add(PILOT_KEY::KEY_LANGUAGE);
        EXPECT_EQ( test_my_data.myEventHandler.run("LED")->getLast1minNumberEvent(),i);
        EXPECT_THAT(test_my_data.myEventHandler.run("LED")->getEvent(),
                    testing::HasSubstr("LED can not start due to home state: UNDEFINE"));
    }
    test_irda->_add(PILOT_KEY::KEY_SUBTITLE);
}

TEST_F(c_irda_logic_fixture, temp_smogINFO)
{
    TEST_DATA::return_httpPost = "ok.\n";
    TEST_DATA::return_send_to_arduino = "12:12";
    test_irda->_add(PILOT_KEY::KEY_MENU);
    do {
        test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    } while(TEST_DATA::LCD_print != "2.TEMPERATURA");

    test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    test_irda->_add(PILOT_KEY::KEY_VOLUMEDOWN);
    test_irda->_add(PILOT_KEY::KEY_OK);
    EXPECT_THAT(TEST_DATA::LCD_print, testing::HasSubstr("TEMPERATURA"));
}

TEST_F(c_irda_logic_fixture, menu_enter_dir)
{
    test_irda->_add(PILOT_KEY::KEY_MENU);
    test_irda->_add(PILOT_KEY::KEY_OK);

    EXPECT_EQ(test_irda->who, PILOT_STATE::MENU);
    test_irda->_add(PILOT_KEY::KEY_UP);
    test_irda->_add(PILOT_KEY::KEY_0); //default
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    EXPECT_EQ(test_irda->who, PILOT_STATE::MPD);
}
TEST_F(c_irda_logic_fixture, menu_files)
{
    blockQueue  test_q;
    test_q._clearAll();
    test_my_data.idom_all_state.houseState = STATE::UNLOCK;
    int timeout = 10;
    test_irda->_add(PILOT_KEY::KEY_MENU);
    do {
        if (--timeout == 0)
            FAIL()<<"cannot find";
        test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    } while(TEST_DATA::LCD_print != "4.PLIKI");
    test_irda->_add(PILOT_KEY::KEY_OK);
    EXPECT_EQ(test_irda->who, PILOT_STATE::MOVIE);
    timeout = 10;
    do {
        if (--timeout == 0)
            FAIL()<<"cannot find";
        test_irda->_add(PILOT_KEY::KEY_VOLUMEDOWN);
    } while(TEST_DATA::LCD_print != "GAME_OF_THRONES/");
    test_irda->_add(PILOT_KEY::KEY_OK);
    test_irda->_add(PILOT_KEY::KEY_UP);
    EXPECT_STREQ(TEST_DATA::LCD_print.c_str(),"GAME_OF_THRONES/");
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    EXPECT_EQ(test_irda->who, PILOT_STATE::MPD);
    ///////////////// play - no MPD

    test_my_data.ptr_MPD_info->isPlay  = false;
    test_irda->_add(PILOT_KEY::KEY_MENU);
    timeout = 10;
    do {
        if (--timeout == 0)
            FAIL()<<"cannot find";
        test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    } while(TEST_DATA::LCD_print != "4.PLIKI");
    test_irda->_add(PILOT_KEY::KEY_OK);
    EXPECT_EQ(test_irda->who, PILOT_STATE::MOVIE);
    timeout = 10;
    do {
        if (--timeout == 0)
            break;
        test_irda->_add(PILOT_KEY::KEY_VOLUMEDOWN);
    } while(TEST_DATA::LCD_print != "s01e02");
    test_irda->_add(PILOT_KEY::KEY_0); //default
    test_irda->_add(PILOT_KEY::KEY_OK);
    EXPECT_EQ(test_irda->who, PILOT_STATE::PROJECTOR);
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    EXPECT_EQ(test_irda->who, PILOT_STATE::MPD);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"),STATE::OFF);
    ///////////////// play -  MPD
    test_my_data.ptr_MPD_info->isPlay  = true;
    EXPECT_TRUE(test_my_data.ptr_MPD_info->isPlay);
    test_irda->_add(PILOT_KEY::KEY_MENU);
    timeout = 10;
    do {
        if (--timeout == 0)
            FAIL()<<"cannot find";
        test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    } while(TEST_DATA::LCD_print != "4.PLIKI");
    test_irda->_add(PILOT_KEY::KEY_OK);
    EXPECT_EQ(test_irda->who, PILOT_STATE::MOVIE);
    timeout = 10;
    do {
        if (--timeout == 0)
            FAIL()<<"cannot find";
        test_irda->_add(PILOT_KEY::KEY_VOLUMEDOWN);
    } while(TEST_DATA::LCD_print != "s01e02");
    test_irda->_add(PILOT_KEY::KEY_OK);
    EXPECT_EQ(test_irda->who, PILOT_STATE::PROJECTOR);

    EXPECT_EQ(test_q._get(), MPD_COMMAND::PAUSE);
}

TEST_F(c_irda_logic_fixture, dummy_KEY_RADIO_and_add )
{
    test_irda->_add(PILOT_KEY::KEY_RADIO);
    EXPECT_EQ(test_irda->who, PILOT_STATE::PROJECTOR);

    test_irda->who = PILOT_STATE::LED; //not set
    test_irda->_add(PILOT_KEY::KEY_RADIO);
}

TEST_F(c_irda_logic_fixture, projektor)
{
    std::string retString;
    test_irda->who = PILOT_STATE::PROJECTOR;
    test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"+");

    test_irda->_add(PILOT_KEY::KEY_VOLUMEDOWN);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"+");

    test_irda->_add(PILOT_KEY::KEY_OK);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"-");

    test_irda->_add(PILOT_KEY::KEY_POWER);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"q");

    test_irda->_add(PILOT_KEY::KEY_DOWN);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"+");

    test_irda->_add(PILOT_KEY::KEY_UP);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"+");

    test_irda->_add(PILOT_KEY::KEY_CHANNELUP);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"o");

    test_irda->_add(PILOT_KEY::KEY_CHANNELDOWN);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
   // EXPECT_EQ(retString,"i");

    test_my_data.ptr_MPD_info->isPlay = true;
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
    EXPECT_EQ(test_irda->who, PILOT_STATE::MPD);

    test_my_data.ptr_MPD_info->isPlay = false;
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    retString = useful_F_libs::read_from_mkfifo(test_omxplayerFile.c_str());
    std::cout << "retString.size() : " << retString.size() << std::endl;
    EXPECT_EQ(test_irda->who, PILOT_STATE::MPD);
    EXPECT_EQ(test_my_data.main_iDomStatus->getObjectState("speakers"),STATE::OFF);

    //// dummy
    test_irda->who = PILOT_STATE::PROJECTOR;
    test_irda->_add(PILOT_KEY::KEY_0);
}

TEST(files_tree, wrong_path){
    LCD_c *mainLCD = NULL;
    auto test_filesTree = new files_tree("../fake/path/", mainLCD);
    EXPECT_EQ(test_filesTree->get_vector_size() , 1);
    delete test_filesTree;
}
