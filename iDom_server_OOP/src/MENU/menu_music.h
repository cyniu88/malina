#ifndef MENU_MUSIC_H
#define MENU_MUSIC_H
#include "menu_base.h"


class MENU_MUSIC : public MENU_STATE_BASE
{
public:
    MENU_MUSIC(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, bool lcdLED = false);
    MENU_MUSIC(const MENU_MUSIC &base);
    MENU_MUSIC(MENU_MUSIC &&base);
    MENU_MUSIC &operator=(const MENU_MUSIC &base);
    MENU_MUSIC &operator=(MENU_MUSIC &&base);
    ~MENU_MUSIC();
    void entry();
    void exit();
    void keyPadPower();
    void keyPadUp();
    void keyPadDown();
    void keyPadLeft();
    void keyPadRight();
    std::string getStateName();
};

#endif // MENU_MUSIC_H
