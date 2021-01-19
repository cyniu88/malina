#include <iostream>
#include "menu_volume.h"

MENU_VOLUME::MENU_VOLUME(thread_data *my_data,LCD_c* lcdPTR, MENU_STATE_MACHINE* msm): MENU_STATE_BASE (my_data, lcdPTR, msm)
{
    std::cout << "MENU_VOLUME::MENU_VOLUME()" << std::endl;
}

MENU_VOLUME::MENU_VOLUME(const MENU_VOLUME &base):MENU_STATE_BASE(base)
{
    std::cout << "MENU_VOLUME::MENU_VOLUME() kopiujacy" << std::endl;
}

MENU_VOLUME::MENU_VOLUME(MENU_VOLUME &&base):MENU_STATE_BASE(std::move(base))
{
    std::cout << "MENU_VOLUME::MENU_VOLUME() przenoszacy" << std::endl;
}

MENU_VOLUME &MENU_VOLUME::operator=(const MENU_VOLUME &base)
{
    std::cout << "MENU_VOLUME::operator = kopiujacy" << std::endl;
     if(&base != this){
         my_dataPTR = base.my_dataPTR;
         lcdPTR = base.lcdPTR;
         stateMachinePTR = base.stateMachinePTR;
     }
     return * this;
}

MENU_VOLUME &MENU_VOLUME::operator=(MENU_VOLUME &&base)
{
    std::cout << "MENU_VOLUME::operator = przenoszacy" << std::endl;
     if(&base != this){
         my_dataPTR = base.my_dataPTR;
         lcdPTR = base.lcdPTR;
         stateMachinePTR = base.stateMachinePTR;
     }
     return * this;
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
