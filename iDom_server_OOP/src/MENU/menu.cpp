#include "menu.h"

MENU_STATE_BASE::MENU_STATE_BASE(STATE_MACHINE *stateMachnie)
{
   this->stateMachnie = stateMachnie;
}

void MENU_STATE_BASE::changeTo(std::unique_ptr<MENU_STATE_BASE> newState)
{
   this->stateMachnie->state = std::move(newState);
}
