#include <iostream>
#include "menu_volume.h"

MENU_VOLUME::MENU_VOLUME()
{
    std::cout << "MENU_VOLUME::MENU_VOLUME()" << std::endl;
}

MENU_VOLUME::~MENU_VOLUME()
{
    std::cout << "MENU_VOLUME::~MENU_VOLUME()" << std::endl;
}

void MENU_VOLUME::entry()
{
    std::cout << "MENU_VOLUME::entry()" << std::endl;
}

void MENU_VOLUME::exit()
{
    std::cout << "MENU_VOLUME::exit()" << std::endl;
}

void MENU_VOLUME::printStateName()
{
    std::cout << "klasa state: " << typeid (this).name() << std::endl;
}
