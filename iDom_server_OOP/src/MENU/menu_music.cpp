#include "menu_music.h"
#include "menu_root.h"
#include "../iDomTools/idomtools_interface.h"

MENU_MUSIC::MENU_MUSIC(thread_data *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) : MENU_STATE_BASE(context, lcdPTR, msm, lcdLED)
{
    // std::cout << "MENU_MUSIC::MENU_MUSIC()" << std::endl;
}

MENU_MUSIC::MENU_MUSIC(const MENU_MUSIC &base) : MENU_STATE_BASE(base)
{
    // std::cout << "MENU_MUSIC::MENU_MUSIC() kopiujacy" << std::endl;
}

MENU_MUSIC::MENU_MUSIC(MENU_MUSIC &&base) : MENU_STATE_BASE(std::move(base))
{
    //  std::cout << "MENU_MUSIC::MENU_MUSIC() przenoszacy" << std::endl;
}

MENU_MUSIC &MENU_MUSIC::operator=(const MENU_MUSIC &base)
{
    //  std::cout << "MENU_MUSIC::operator = kopiujacy" << std::endl;
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

MENU_MUSIC &MENU_MUSIC::operator=(MENU_MUSIC &&base)
{
    //  std::cout << "MENU_MUSIC::operator = przenoszacy" << std::endl;
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

void MENU_MUSIC::entry()
{
    contextPTR->main_iDomTools->MPD_play(contextPTR);
    print("Gram muzyczke");
}

void MENU_MUSIC::exit()
{
}

void MENU_MUSIC::keyPadPower()
{
    contextPTR->main_iDomTools->MPD_stop();
    changeStateTo<MENU_ROOT>();
}

void MENU_MUSIC::keyPadUp()
{
    contextPTR->main_iDomTools->MPD_volumeUp();
}

void MENU_MUSIC::keyPadDown()
{
    contextPTR->main_iDomTools->MPD_volumeDown();
}

void MENU_MUSIC::keyPadLeft()
{
    contextPTR->main_iDomTools->MPD_prev();
}

void MENU_MUSIC::keyPadRight()
{
    contextPTR->main_iDomTools->MPD_next();
}

std::string MENU_MUSIC::getStateName() const
{
    return typeid(this).name();
}
