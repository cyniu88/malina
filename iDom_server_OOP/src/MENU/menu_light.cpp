#include <iostream>

#include "menu_light.h"
#include "menu_main.h"
#include "menu_volume.h"
#include "menu_root.h"

MENU_LIGHT::MENU_LIGHT(thread_data *my_data):MENU_STATE_BASE (my_data)
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

std::string MENU_LIGHT::getStateName()
{
    return typeid (this).name();
}

void MENU_LIGHT::keyPadRes()
{
    changeTo<MENU_ROOT>();
}

