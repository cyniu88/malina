#include "menu_root.h"
#include "menu_main.h"

#include "../RADIO_433_eq/radio_433_eq.h"
#include "../iDomTools/idomtools_interface.h"

MENU_ROOT::MENU_ROOT(thread_data *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) : MENU_STATE_BASE(context, lcdPTR, msm, lcdLED),
                                                                                                   tempCounter(0)
{
}

MENU_ROOT::MENU_ROOT(const MENU_ROOT &base) : MENU_STATE_BASE(base),
                                              tempCounter(base.tempCounter)
{
}

MENU_ROOT::MENU_ROOT(MENU_ROOT &&base) : MENU_STATE_BASE(std::move(base)),
                                         tempCounter(base.tempCounter)
{
}

MENU_ROOT &MENU_ROOT::operator=(const MENU_ROOT &base)
{
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        tempCounter = base.tempCounter;
    }
    return *this;
}

MENU_ROOT &MENU_ROOT::operator=(MENU_ROOT &&base)
{
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        tempCounter = base.tempCounter;
    }
    return *this;
}

void MENU_ROOT::entry()
{
    if (contextPTR->main_iDomStatus->getObjectState("music") == STATE::STOP)
    {
        lcdPTR->noBacklight();
    }
    else
    {
        quickPrint(contextPTR->ptr_MPD_info->artist, contextPTR->ptr_MPD_info->radio);
    }
    tempCounter = 0;
}

void MENU_ROOT::exit()
{
}

std::string MENU_ROOT::getStateName() const
{
    return typeid(this).name();
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
    if (tempCounter == 0)
    {
        ss << std::setprecision(4) << contextPTR->ptr_buderus->getInsideTemp()
           << (char)223 << "c  " << contextPTR->lusina.shedTemp.average() << ((char)223) << "c";
        quickPrint("T: salon  biuro", ss.str());
    }
    else if (tempCounter == 1)
    {
        RADIO_WEATHER_STATION *st = static_cast<RADIO_WEATHER_STATION *>(contextPTR->main_REC->getEqPointer("first"));
        auto temp = st->data.getTemperature();
        ss << std::setprecision(4) << temp << celsiusDegrees << " " << contextPTR->ptr_buderus->getOutdoorTemp() << celsiusDegrees;
        quickPrint("T: domek   pole", ss.str());
    }
    else if (tempCounter == 2)
    {
        ss << std::setprecision(4) << contextPTR->lusina.shedHum.average() << "%   " << contextPTR->lusina.shedPres.average() << "hPa";
        quickPrint("Wilgoc   Cis", ss.str());
    }
    else if (tempCounter == 3)
    {
        ss << std::setprecision(4) << contextPTR->lusina.shedFloor.average() << celsiusDegrees;
        quickPrint("Podloga", ss.str());
    }
    else
    {
        ss << std::setprecision(4) << contextPTR->ptr_buderus->getBoilerTemp()
           << celsiusDegrees << "    " << contextPTR->ptr_buderus->getCurFlowTemp() << celsiusDegrees;
        quickPrint("boiler   curFlow", ss.str());
        tempCounter = -1;
    }
    tempCounter++;
    contextPTR->main_Rs232->print("TIMEOUT:9000;");
}

void MENU_ROOT::keyPadRes()
{
    this->scrollText();
}

void MENU_ROOT::reboot()
{
    contextPTR->main_iDomTools->reloadHard_iDomServer();
}

void MENU_ROOT::timeout(std::function<void()> function)
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
    _row1 = row1;
    _row2 = row2;
    numberOfCharCounter = row1.size() - numberOfChar;
}

void MENU_ROOT::volumeUp()
{
    contextPTR->main_iDomTools->MPD_volumeUp();
    contextPTR->main_Rs232->print("TIMEOUT:3000;");
}

void MENU_ROOT::volumeDown()
{
    contextPTR->main_iDomTools->MPD_volumeDown();
    contextPTR->main_Rs232->print("TIMEOUT:3000;");
}

void MENU_ROOT::mpdStartStop()
{
    if (contextPTR->main_iDomStatus->getObjectState("music") == STATE::STOP)
    {
        contextPTR->main_iDomTools->MPD_play(contextPTR);
    }
    else
    {
        contextPTR->main_iDomTools->MPD_stop();
        lcdPTR->clear();
        lcdPTR->noBacklight();
    }
}

void MENU_ROOT::mpdNext()
{
    contextPTR->main_iDomTools->MPD_next();
}

void MENU_ROOT::mpdPrev()
{
    contextPTR->main_iDomTools->MPD_prev();
}

void MENU_ROOT::scrollText()
{
    if ((Clock::getUnixTime() - time) < offset)
        return;
    time = Clock::getUnixTime();
    offset = 1;
    if (_row1.size() > numberOfChar)
    {
        if (numberOfCharCounter != 0)
        {
            lcdPTR->scrollLeft();
            --numberOfCharCounter;
        }
        else
        {
            offset = 4;
            quickPrint(this->_row1, this->_row2);
        }
    }
}
