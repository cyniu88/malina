#include <iostream>

#include "menu_light.h"
#include "menu_main.h"
#include "menu_volume.h"
#include "menu_root.h"

MENU_LIGHT::MENU_LIGHT(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm):MENU_STATE_BASE (my_data, lcdPTR, msm)
{
    std::cout << "MENU_LIGHT::MENU_LIGHT()" << std::endl;
}

MENU_LIGHT::MENU_LIGHT(const MENU_LIGHT &base): MENU_STATE_BASE(base)
{
    std::cout << "MENU_LIGHT::MENU_LIGHT() kopiujacy" << std::endl;
}

MENU_LIGHT::MENU_LIGHT(MENU_LIGHT &&base):MENU_STATE_BASE(base)
{
    std::cout << "MENU_LIGHT::MENU_LIGHT() przenoszacy" << std::endl;
}

MENU_LIGHT &MENU_LIGHT::operator=(const MENU_LIGHT &base)
{
    std::cout << "MENU_LIGHT::operator = kopiujacy" << std::endl;
    if(&base != this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_LIGHT &MENU_LIGHT::operator=(MENU_LIGHT &&base)
{
    std::cout << "MENU_LIGHT:operator = przenoszacy" << std::endl;
    if(&base != this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
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

