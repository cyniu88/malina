#ifndef MENU_ROOT_H
#define MENU_ROOT_H

#include "menu_base.h"

class MENU_ROOT : public MENU_STATE_BASE
{
public:
    MENU_ROOT(std::shared_ptr<LCD_c> _lcdPtr);
    MENU_ROOT() = default;
    ~MENU_ROOT() = default;
    void entry();
    void exit();
    void printStateName();
    void keyPadMenu();
};

#endif // MENU_ROOT_H
