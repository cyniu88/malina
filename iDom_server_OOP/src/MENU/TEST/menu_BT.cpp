#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "../../iDom_server_OOP.h"
#include "../menu_base.h"
#include "../menu_root.h"
#include "../menu_main.h"

class menu_state_machine_fixture: public testing::Test
{
public:

protected:

    virtual void SetUp() final
    {

    }

    virtual void TearDown() final
    {

    }
};

TEST_F(menu_state_machine_fixture, containerMenuMap)
{
    cyniu::ContainerMenuMap<std::string, int> testMenuMap;
    testMenuMap.insert(std::pair<std::string, int>("a jeden",1));
    testMenuMap.insert(std::pair<std::string, int>("b dwa",2));
    testMenuMap.insert(std::pair<std::string, int>("c trzy",3));
    testMenuMap.begin();

    {auto para  = testMenuMap.getCurrent();
        EXPECT_THAT(para.second, 1);}
    testMenuMap.up();

    {auto para  = testMenuMap.getCurrent();
        EXPECT_THAT(para.second, 2);}
    testMenuMap.up();

    {auto para  = testMenuMap.getCurrent();
        EXPECT_THAT(para.second, 3);}

    testMenuMap.down();
    {auto para  = testMenuMap.getCurrent();
        EXPECT_THAT(para.second, 2);}

    testMenuMap.down();
    {auto para  = testMenuMap.getCurrent();
        EXPECT_THAT(para.second, 1);}

    testMenuMap.down();
    {auto para  = testMenuMap.getCurrent();
        EXPECT_THAT(para.second, 3);}
}

TEST_F(menu_state_machine_fixture, main)
{
    thread_data test_my_data;
    LCD_c lcd(0x27,16,2);
    MENU_STATE_MACHINE stateMechine;
    auto ptr = std::make_unique<MENU_ROOT>(&test_my_data, &lcd, &stateMechine);
    stateMechine.setStateMachine(std::move(ptr));
    auto main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);

    auto res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_ROOT"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::MENU);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_MAIN"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);

    main_key_menu_handler->recKeyEvent(KEY_PAD::RES);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_ROOT"));
}
