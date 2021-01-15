#ifndef MENU_ROOT_H
#define MENU_ROOT_H

#include "menu_base.h"

class MENU_ROOT : public MENU_STATE_BASE
{
public:
    MENU_ROOT(thread_data *my_data);
    ~MENU_ROOT() = default;
    void entry();
    void exit();
    void printStateName();
    void keyPadMenu();
};

#endif // MENU_ROOT_H
