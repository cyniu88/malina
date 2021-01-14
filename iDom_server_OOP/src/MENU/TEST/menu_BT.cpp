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
    std::unique_ptr<KEY_HANDLER> mainHandler = std::make_unique<KEY_HANDLER>();
    mainHandler->recKeyEvent(KEY_PAD::MENU);
    mainHandler->recKeyEvent(KEY_PAD::OK);

}
