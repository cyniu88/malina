#include <iostream>
#include "menu_main.h"
#include "menu_light.h"
#include "menu_root.h"

MENU_MAIN::MENU_MAIN(thread_data* my_data): MENU_STATE_BASE (my_data)
{
    std::cout << "MENU_MAIN::MENU_MAIN()" << std::endl;
}

MENU_MAIN::~MENU_MAIN()
{
    std::cout << "MENU_MAIN::~MENU_MAIN()" << std::endl;
}

void MENU_MAIN::entry()
{
    std::cout << "MENU_MAIN::entry()" << std::endl;
    my_dataPTR->mainLCD->printString("TEST1","MAIN");
}

void MENU_MAIN::exit()
{
    std::cout << "MENU_MAIN::exit()" << std::endl;
}

std::string MENU_MAIN::getStateName()
{
    return typeid (this).name();
}

void MENU_MAIN::keyPadOk()
{
     changeTo<MENU_LIGHT>();
}

void MENU_MAIN::keyPadRes()
{
   changeTo<MENU_ROOT>();
}

