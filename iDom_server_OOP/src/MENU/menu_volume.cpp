#include <iostream>
#include "menu_volume.h"

MENU_VOLUME::MENU_VOLUME(thread_context *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) : MENU_STATE_BASE(context, lcdPTR, msm, lcdLED)
{
    // std::cout << "MENU_VOLUME::MENU_VOLUME()" << std::endl;
}

MENU_VOLUME::MENU_VOLUME(const MENU_VOLUME &base) : MENU_STATE_BASE(base)
{
    // std::cout << "MENU_VOLUME::MENU_VOLUME() kopiujacy" << std::endl;
}

MENU_VOLUME::MENU_VOLUME(MENU_VOLUME &&base) : MENU_STATE_BASE(std::move(base))
{
    //  std::cout << "MENU_VOLUME::MENU_VOLUME() przenoszacy" << std::endl;
}

MENU_VOLUME &MENU_VOLUME::operator=(const MENU_VOLUME &base)
{
    //  std::cout << "MENU_VOLUME::operator = kopiujacy" << std::endl;
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

MENU_VOLUME &MENU_VOLUME::operator=(MENU_VOLUME &&base)
{
    //  std::cout << "MENU_VOLUME::operator = przenoszacy" << std::endl;
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

void MENU_VOLUME::entry()
{
}

void MENU_VOLUME::exit()
{
}

std::string MENU_VOLUME::getStateName() const
{
    return typeid(this).name();
}
