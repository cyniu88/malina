#ifndef MENU_KODI_H
#define MENU_KODI_H
#include "menu_base.h"

class MENU_KODI : public MENU_STATE_BASE
{
public:
    MENU_KODI(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_KODI(const MENU_KODI &base);
    MENU_KODI(MENU_KODI &&base);
    MENU_KODI &operator=(const MENU_KODI &base);
    MENU_KODI &operator=(MENU_KODI &&base);
    ~MENU_KODI();
    void entry();
    void exit();
    std::string getStateName();
    void timeout(std::function<void()> function = std::nullptr_t());
};

#endif // MENU_KODI_H
