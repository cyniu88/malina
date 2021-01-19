#ifndef MENU_LIGHT_H
#define MENU_LIGHT_H
#include "menu_base.h"

class MENU_LIGHT : public MENU_STATE_BASE
{
public:
    MENU_LIGHT(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm);
    MENU_LIGHT(const MENU_LIGHT & base);
    MENU_LIGHT(MENU_LIGHT && base);
    MENU_LIGHT &operator= (const MENU_LIGHT& base);
    MENU_LIGHT &operator= (MENU_LIGHT &&base);
    ~MENU_LIGHT();
    void entry();
    void exit();
    std::string getStateName();
    void keyPadRes();
};

#endif // MENU_LIGHT_H
