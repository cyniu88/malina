#include <iostream>

#include "menu_light.h"
#include "menu_main.h"
#include "menu_volume.h"

MENU_LIGHT::MENU_LIGHT()
{
    std::cout << "MENU_LIGHT::MENU_LIGHT()" << std::endl;
}

MENU_LIGHT::~MENU_LIGHT()
{
    std::cout << "MENU_LIGHT::~MENU_LIGHT()" << std::endl;
}

void MENU_LIGHT::entry()
{
    std::cout << "MENU_LIGHT::entry()" << std::endl;
}

void MENU_LIGHT::exit()
{
    std::cout << "MENU_LIGHT::exit()" << std::endl;
}

void MENU_LIGHT::printStateName()
{
    std::cout << "klasa state: " << typeid (this).name() << std::endl;
}

