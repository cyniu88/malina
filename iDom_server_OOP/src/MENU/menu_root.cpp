#include "menu_root.h"
#include "menu_main.h"
#include "menu_music.h"

MENU_ROOT::MENU_ROOT(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, bool lcdLED): MENU_STATE_BASE (my_data, lcdPTR, msm, lcdLED)
{
    //  std::cout << "MENU_ROOT::MENU_ROOT()" << std::endl;
}

MENU_ROOT::MENU_ROOT(const MENU_ROOT &base): MENU_STATE_BASE(base)
{
    //  std::cout << "MENU_ROOT::MENU_ROOT() kopiujacy" << std::endl;
}

MENU_ROOT::MENU_ROOT(MENU_ROOT &&base):MENU_STATE_BASE(std::move(base))
{
    //  std::cout << "MENU_ROOT::MENU_ROOT() przenoszacy" << std::endl;
}

MENU_ROOT &MENU_ROOT::operator=(const MENU_ROOT &base)
{
    //  std::cout << "MENU_ROOT::operator = kopiujacy" << std::endl;
    if(&base != this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_ROOT::~MENU_ROOT()
{
    //  std::cout << "MENU_ROOT::~MENU_ROOT()" << std::endl;
}

MENU_ROOT &MENU_ROOT::operator=(MENU_ROOT &&base)
{
    // std::cout << "MENU_ROOT::operator = przenoszacy" << std::endl;
    if(&base != this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

void MENU_ROOT::entry()
{
    if(lcdLED == false)
        lcdPTR->noBacklight();
}

void MENU_ROOT::exit()
{

}

std::string MENU_ROOT::getStateName()
{
    return typeid (this).name();
}

void MENU_ROOT::keyPadMenu()
{
    changeStateTo<MENU_MAIN>();
}

void MENU_ROOT::keyPadPower()
{
    if(my_dataPTR->main_iDomStatus->getObjectState("music") == STATE::STOP){
        my_dataPTR->main_iDomTools->MPD_play(my_dataPTR);
        print("Gram muzyczke");
    }
    else{
        my_dataPTR->main_iDomTools->MPD_stop();
        lcdPTR->clear();
        lcdPTR->noBacklight();
    }
}

void MENU_ROOT::keyPadUp()
{
    my_dataPTR->main_iDomTools->MPD_volumeUp();
}

void MENU_ROOT::keyPadDown()
{
    my_dataPTR->main_iDomTools->MPD_volumeDown();
}

void MENU_ROOT::keyPadLeft()
{
    my_dataPTR->main_iDomTools->MPD_prev();
}

void MENU_ROOT::keyPadRight()
{
    my_dataPTR->main_iDomTools->MPD_next();
}

void MENU_ROOT::reboot()
{
    my_dataPTR->main_iDomTools->reloadHard_iDomServer();
}

void MENU_ROOT::quickPrint(const std::string &row1, const std::string &row2)
{
    lcdPTR->clear();
    lcdPTR->printString(row1, row2);
}
