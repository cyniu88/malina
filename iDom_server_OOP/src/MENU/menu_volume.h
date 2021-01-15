#ifndef MENU_VOLUME_H
#define MENU_VOLUME_H
#include "menu_base.h"

class MENU_VOLUME : public MENU_STATE_BASE
{
public:
    MENU_VOLUME(thread_data *my_data);
    ~MENU_VOLUME();
    void entry();
    void exit();
    void printStateName();
};

#endif // MENU_VOLUME_H
