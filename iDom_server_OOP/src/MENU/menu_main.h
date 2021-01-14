#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include "menu_base.h"

class MENU_MAIN : public MENU_STATE_BASE
{
public:
    MENU_MAIN();
    ~MENU_MAIN();
    void entry();
    void exit();
    void printStateName();
    void keyPadOk();
    void keyPadRes();
};

#endif // MENU_MAIN_H
