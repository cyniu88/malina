#include "menu_music.h"
#include "menu_root.h"

MENU_MUSIC::MENU_MUSIC(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED): MENU_STATE_BASE (my_data, lcdPTR, msm, lcdLED)
{
    // std::cout << "MENU_MUSIC::MENU_MUSIC()" << std::endl;
}

MENU_MUSIC::MENU_MUSIC(const MENU_MUSIC &base):MENU_STATE_BASE(base)
{
    // std::cout << "MENU_MUSIC::MENU_MUSIC() kopiujacy" << std::endl;
}

MENU_MUSIC::MENU_MUSIC(MENU_MUSIC &&base):MENU_STATE_BASE(std::move(base))
{
    //  std::cout << "MENU_MUSIC::MENU_MUSIC() przenoszacy" << std::endl;
}

MENU_MUSIC &MENU_MUSIC::operator=(const MENU_MUSIC &base)
{
    //  std::cout << "MENU_MUSIC::operator = kopiujacy" << std::endl;
    if(&base not_eq this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_MUSIC &MENU_MUSIC::operator=(MENU_MUSIC &&base)
{
    //  std::cout << "MENU_MUSIC::operator = przenoszacy" << std::endl;
    if(&base not_eq this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_MUSIC::~MENU_MUSIC()
{
}

void MENU_MUSIC::entry()
{
    my_dataPTR->main_iDomTools->MPD_play(my_dataPTR);
    print("Gram muzyczke");
}

void MENU_MUSIC::exit()
{

}

void MENU_MUSIC::keyPadPower()
{
    my_dataPTR->main_iDomTools->MPD_stop();
    changeStateTo<MENU_ROOT>();
}

void MENU_MUSIC::keyPadUp()
{
    my_dataPTR->main_iDomTools->MPD_volumeUp();
}

void MENU_MUSIC::keyPadDown()
{
    my_dataPTR->main_iDomTools->MPD_volumeDown();
}

void MENU_MUSIC::keyPadLeft()
{
    my_dataPTR->main_iDomTools->MPD_prev();
}

void MENU_MUSIC::keyPadRight()
{
    my_dataPTR->main_iDomTools->MPD_next();
}

std::string MENU_MUSIC::getStateName() const
{
    return typeid (this).name();
}
