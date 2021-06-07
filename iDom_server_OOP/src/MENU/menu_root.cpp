#include "menu_root.h"
#include "menu_main.h"
#include "menu_music.h"

MENU_ROOT::MENU_ROOT(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED):
    MENU_STATE_BASE (my_data, lcdPTR, msm, lcdLED),
    tempCounter(0)
{
}

MENU_ROOT::MENU_ROOT(const MENU_ROOT &base): MENU_STATE_BASE(base),
    tempCounter(base.tempCounter)
{
}

MENU_ROOT::MENU_ROOT(MENU_ROOT &&base):MENU_STATE_BASE(std::move(base)),
    tempCounter(base.tempCounter)
{
}

MENU_ROOT &MENU_ROOT::operator=(const MENU_ROOT &base)
{
    if(&base not_eq this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        tempCounter = base.tempCounter;
    }
    return * this;
}

MENU_ROOT::~MENU_ROOT()
{
}

MENU_ROOT &MENU_ROOT::operator=(MENU_ROOT &&base)
{
    if(&base not_eq this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        tempCounter = base.tempCounter;
    }
    return * this;
}

void MENU_ROOT::entry()
{
    if(my_dataPTR->main_iDomStatus->getObjectState("music") == STATE::STOP)
        lcdPTR->noBacklight();
    else
    {
        quickPrint(my_dataPTR->ptr_MPD_info->artist, my_dataPTR->ptr_MPD_info->radio);
    }
    tempCounter = 0;
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
    mpdStartStop();
}

void MENU_ROOT::keyPadUp()
{
    volumeUp();
}

void MENU_ROOT::keyPadDown()
{
    volumeDown();
}

void MENU_ROOT::keyPadLeft()
{
    mpdPrev();
}

void MENU_ROOT::keyPadRight()
{
    mpdNext();
}

void MENU_ROOT::keyPadEpg()
{
    std::stringstream ss;
    if(tempCounter == 0){
        ss << std::setprecision(4) << my_dataPTR->ptr_buderus->getInsideTemp()
           << "      " << my_dataPTR->ptr_buderus->getOutdoorTemp();
        quickPrint("Temp: in    out", ss.str());
        tempCounter++;
    }
    else{
        ss << std::setprecision(4) << my_dataPTR->ptr_buderus->getBoilerTemp()
           << "      " << my_dataPTR->ptr_buderus->getCurFlowTemp();
        quickPrint("boiler   curFlow", ss.str());
    }
    my_dataPTR->main_Rs232->print("TIMEOUT:3000;");
}

void MENU_ROOT::reboot()
{
    my_dataPTR->main_iDomTools->reloadHard_iDomServer();
}

void MENU_ROOT::timeout(std::function<void ()> function)
{
    entry();
}

void MENU_ROOT::offLcd()
{
    lcdPTR->clear();
    lcdPTR->noBacklight();
}

void MENU_ROOT::quickPrint(const std::string &row1, const std::string &row2)
{
    lcdPTR->clear();
    lcdPTR->printString(row1, row2);
}

void MENU_ROOT::volumeUp()
{
    my_dataPTR->main_iDomTools->MPD_volumeUp();
    my_dataPTR->main_Rs232->print("TIMEOUT:3000;");
}

void MENU_ROOT::volumeDown()
{
    my_dataPTR->main_iDomTools->MPD_volumeDown();
    my_dataPTR->main_Rs232->print("TIMEOUT:3000;");
}

void MENU_ROOT::mpdStartStop()
{
    if(my_dataPTR->main_iDomStatus->getObjectState("music") == STATE::STOP){
        my_dataPTR->main_iDomTools->MPD_play(my_dataPTR);
    }
    else{
        my_dataPTR->main_iDomTools->MPD_stop();
        lcdPTR->clear();
        lcdPTR->noBacklight();
    }
}

void MENU_ROOT::mpdNext()
{
    my_dataPTR->main_iDomTools->MPD_next();
}

void MENU_ROOT::mpdPrev()
{
    my_dataPTR->main_iDomTools->MPD_prev();
}
