#ifndef MENU_STATE_MACHINE_H
#define MENU_STATE_MACHINE_H

#include <memory>
#include "menu_base.h"

class MENU_STATE_MACHINE
{
public:
    MENU_STATE_MACHINE();
    std::unique_ptr<MENU_STATE_BASE> stateMachnie;
};

#endif // MENU_STATE_MACHINE_H
