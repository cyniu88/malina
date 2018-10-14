#include <gtest/gtest.h>
#include "../../iDom_server_OOP.h"
#include "test_data.h"
#include "../../../iDom_server_OOP/src/iDomTools/test/iDomTools_fixture.h"
#include "../c_irda_logic.h"


class c_irda_logic_fixture : public iDomTOOLS_ClassTest
{
public:
    c_irda_logic* test_irda;
    menu_tree* test_menuTree;

    void SetUp()
    {
        iDomTOOLS_ClassTest::SetUp();
        test_irda = new c_irda_logic(&test_my_data);
        test_menuTree = new menu_tree("../config/MENU/", test_my_data.mainLCD);

        test_my_data.main_MENU = test_menuTree;
        std::cout << "c_irda_logic_fixture SetUp()"<<std::endl;
    }

    void TearDown()
    {
        delete test_irda;
        delete test_menuTree;
        iDomTOOLS_ClassTest::TearDown();
        std::cout << "c_irda_logic_fixture TearDown()"<<std::endl;
    }
};

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

TEST_F(c_irda_logic_fixture, sleeperLogic)
{
    Thread_array_struc test_ThreadArrayStruc[iDomConst::MAX_CONNECTION];

    for (int i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
        test_ThreadArrayStruc[i].thread_socket = i+1;

    test_my_data.main_THREAD_arr = test_ThreadArrayStruc;

    test_my_data.sleeper = 0;
    EXPECT_EQ(test_my_data.sleeper, 0);
    test_irda->_add(PILOT_KEY::KEY_MENU);
    test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    //test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
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
    EXPECT_EQ(test_my_data.sleeper, 1);
    test_irda->_add(PILOT_KEY::KEY_EXIT);
    EXPECT_EQ(test_my_data.sleeper, 0);
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
    // EXPECT_STREQ(TEST_DATA::serial_sended.c_str(), "GASZE LEDy");
}
TEST_F(c_irda_logic_fixture, temp_smogINFO)
{
    test_irda->_add(PILOT_KEY::KEY_MENU);
    test_irda->_add(PILOT_KEY::KEY_VOLUMEUP);
    test_irda->_add(PILOT_KEY::KEY_OK);
    EXPECT_THAT(TEST_DATA::LCD_print, testing::HasSubstr("TEMPERATURA"));
}
