#include "menu_state_machine.h"
#include "menu_main.h"

MENU_STATE_MACHINE::MENU_STATE_MACHINE()
{
   stateMachnie = std::make_unique<MENU_MAIN>();
}
