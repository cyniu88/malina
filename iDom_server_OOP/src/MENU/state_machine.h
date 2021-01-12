#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "menu.h"

class STATE_MACHINE
{
public:
    STATE_MACHINE();
    std::unique_ptr<MENU_STATE_BASE> state = std::nullptr_t();
};

#endif // STATE_MACHINE_H
