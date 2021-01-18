#include <iostream>
#include "menu_base.h"
#include "menu_root.h"

std::unique_ptr<MENU_STATE_BASE_IMPL> MENU_STATE_BASE::ptr = std::nullptr_t(); //std::make_shared<MENU_STATE_BASE>();
MENU_STATE_BASE::MENU_STATE_BASE(thread_data *my_data)
{
    this->my_dataPTR = my_data;
    std::cout << "MENU_STATE_BASE::MENU_STATE_BASE()" << std::endl;
}

KEY_HANDLER::KEY_HANDLER(thread_data *my_data)
{
    MENU_STATE_BASE::ptr = std::make_unique<MENU_ROOT>(my_data);
    MENU_STATE_BASE::ptr->entry();
}

void KEY_HANDLER::recKeyEvent(KEY_PAD eventId)
{
    switch (eventId) {
    case KEY_PAD::OK : MENU_STATE_BASE::ptr->keyPadOk();
        break;
    case KEY_PAD::MENU: MENU_STATE_BASE::ptr->keyPadMenu();
        break;
    case KEY_PAD::POWER: MENU_STATE_BASE::ptr->keyPadPower();
        break;
    case KEY_PAD::RES: MENU_STATE_BASE::ptr->keyPadRes();
        break;
    case KEY_PAD::EPG: MENU_STATE_BASE::ptr->keyPadEpg();
        break;
    case KEY_PAD::UP: MENU_STATE_BASE::ptr->keyPadUp();
        break;
    case KEY_PAD::DOWN: MENU_STATE_BASE::ptr->keyPadDown();
        break;
    case KEY_PAD::RIGHT: MENU_STATE_BASE::ptr->keyPadRight();
        break;
    case KEY_PAD::LEFT: MENU_STATE_BASE::ptr->keyPadLeft();
        break;

    }
}
