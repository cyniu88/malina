#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "../../iDom_server_OOP.h"
#include "../menu_main.h"
#include "../menu_light.h"
#include "../menu_state_machine.h"

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
    MENU_STATE_BASE::ptr = std::make_shared<MENU_MAIN>();
    //main_menu->ptr = main_menu;
    MENU_STATE_BASE::ptr->printStateName();
    MENU_STATE_BASE::ptr->cyniu();
    MENU_STATE_BASE::ptr->printStateName();
    MENU_STATE_BASE::ptr->cyniu();
    MENU_STATE_BASE::ptr->printStateName();

}
