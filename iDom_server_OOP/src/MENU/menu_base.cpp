#include "menu_base.h"
#include <iostream>

MENU_STATE_BASE::MENU_STATE_BASE()
{
}

void MENU_STATE_BASE::cyniu()
{
    std::cout << "brak implementacji cyniu()" << std::endl;
}

std::shared_ptr<MENU_STATE_BASE_IMPL> MENU_STATE_BASE::ptr = std::nullptr_t(); //std::make_shared<MENU_STATE_BASE>();
