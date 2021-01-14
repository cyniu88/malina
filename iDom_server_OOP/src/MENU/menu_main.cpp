#include <iostream>
#include "menu_main.h"
#include "menu_light.h"
#include "menu_root.h"

MENU_MAIN::MENU_MAIN()
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
    lcd->printString("TEST1","MAIN");
}

void MENU_MAIN::exit()
{
    std::cout << "MENU_MAIN::exit()" << std::endl;
}

void MENU_MAIN::printStateName()
{
    std::cout << "klasa state: " << typeid (this).name() << std::endl;
}

void MENU_MAIN::keyPadOk()
{
     changeTo<MENU_LIGHT>();
}

void MENU_MAIN::keyPadRes()
{
   changeTo<MENU_ROOT>();
}

