#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "../../iDom_server_OOP.h"
#include "../menu_base.h"

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
    LCD_c test_lcd(0x27,16,2);
    test_my_data.mainLCD = &test_lcd;

    std::unique_ptr<KEY_HANDLER> mainHandler = std::make_unique<KEY_HANDLER>(&test_my_data);
    mainHandler->recKeyEvent(KEY_PAD::MENU);
    std::cout << "moja klasa: " << MENU_STATE_BASE::ptr->getStateName() << std::endl;
    mainHandler->recKeyEvent(KEY_PAD::OK);
    mainHandler->recKeyEvent(KEY_PAD::RES);
    std::cout << "moja klasa: " << MENU_STATE_BASE::ptr->getStateName() << std::endl;
}
