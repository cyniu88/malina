#ifndef MENU_LIGHT_H
#define MENU_LIGHT_H
#include "menu_base.h"

class MENU_LIGHT : public MENU_STATE_BASE
{
public:
    MENU_LIGHT();
    ~MENU_LIGHT();
    void entry();
    void exit();
    void printStateName();
};

#endif // MENU_LIGHT_H
