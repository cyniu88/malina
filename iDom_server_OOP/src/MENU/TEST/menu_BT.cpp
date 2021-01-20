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
    stateMechine.setStateMachine<MENU_ROOT>(&test_my_data,&lcd, &stateMechine);
    auto main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);

    main_key_menu_handler->recKeyEvent(KEY_PAD::MENU);
    std::cout << "moja klasa: " << stateMechine.currentState->getStateName() << std::endl;
    main_key_menu_handler->recKeyEvent(KEY_PAD::OK);
    main_key_menu_handler->recKeyEvent(KEY_PAD::RES);
}
