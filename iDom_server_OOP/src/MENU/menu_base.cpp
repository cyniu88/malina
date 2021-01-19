#include <iostream>
#include "menu_base.h"
#include "menu_root.h"


MENU_STATE_BASE::MENU_STATE_BASE(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm)
{
    this->my_dataPTR = my_data;
    this->lcdPTR = lcdPTR;
    this->stateMachinePTR = msm;
    std::cout << "MENU_STATE_BASE::MENU_STATE_BASE()" << std::endl;
}

MENU_STATE_BASE::MENU_STATE_BASE(const MENU_STATE_BASE &base): my_dataPTR(base.my_dataPTR), lcdPTR(base.lcdPTR), stateMachinePTR(base.stateMachinePTR)
{
    std::cout << "MENU_STATE_BASE::MENU_STATE_BASE() kopiujacy" << std::endl;
}

MENU_STATE_BASE::MENU_STATE_BASE(const MENU_STATE_BASE &&base)
{
    std::cout << "MENU_STATE_BASE::MENU_STATE_BASE() przenoszacy" << std::endl;
    my_dataPTR = std::move(base.my_dataPTR);
    lcdPTR = std::move(base.lcdPTR);
    stateMachinePTR = std::move(base.stateMachinePTR);
}

MENU_STATE_BASE &MENU_STATE_BASE::operator =(const MENU_STATE_BASE &base)
{
    std::cout << "MENU_STATE_BASE::operator = kopiujacy" << std::endl;
    if (&base != this) {
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_STATE_BASE &MENU_STATE_BASE::operator =(MENU_STATE_BASE &&base)
{
    std::cout << "MENU_STATE_BASE::operator = przenoszacy" << std::endl;
    if (&base != this) {
        my_dataPTR = std::move(base.my_dataPTR);
        lcdPTR = std::move(base.lcdPTR);
        stateMachinePTR = std::move(base.stateMachinePTR);
    }
    return * this;
}

KEY_HANDLER::KEY_HANDLER( MENU_STATE_MACHINE *msm)
{
    this->stateMachinePtr = msm;
}

void KEY_HANDLER::recKeyEvent(KEY_PAD eventId)
{
    switch (eventId) {
    case KEY_PAD::OK : stateMachinePtr->currentState->keyPadOk();
        break;
    case KEY_PAD::MENU: stateMachinePtr->currentState->keyPadMenu();
        break;
    case KEY_PAD::POWER: stateMachinePtr->currentState->keyPadPower();
        break;
    case KEY_PAD::RES: stateMachinePtr->currentState->keyPadRes();
        break;
    case KEY_PAD::EPG: stateMachinePtr->currentState->keyPadEpg();
        break;
    case KEY_PAD::UP: stateMachinePtr->currentState->keyPadUp();
        break;
    case KEY_PAD::DOWN: stateMachinePtr->currentState->keyPadDown();
        break;
    case KEY_PAD::RIGHT: stateMachinePtr->currentState->keyPadRight();
        break;
    case KEY_PAD::LEFT: stateMachinePtr->currentState->keyPadLeft();
        break;
    }
}
