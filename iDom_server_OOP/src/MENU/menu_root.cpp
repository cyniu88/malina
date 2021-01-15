#include "menu_root.h"
#include "menu_main.h"

MENU_ROOT::MENU_ROOT(thread_data *my_data): MENU_STATE_BASE (my_data)
{
}

void MENU_ROOT::entry()
{
    my_dataPTR->mainLCD->noBacklight();
}

void MENU_ROOT::exit()
{

}

std::string MENU_ROOT::getStateName()
{
    return typeid (this).name();
}

void MENU_ROOT::keyPadMenu()
{
    changeTo<MENU_MAIN>();
}
