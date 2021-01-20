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

    main_key_menu_handler->recKeyEvent(KEY_PAD::OK);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_LIGHT"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::RES);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_ROOT"));

}
