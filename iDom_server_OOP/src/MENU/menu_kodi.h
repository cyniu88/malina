#ifndef MENU_KODI_H
#define MENU_KODI_H
#include "menu_base.h"

class MENU_KODI : public MENU_STATE_BASE
{
    int kodiVolume = 50;
    int kodiMsgId = 8;
    std::string kodiHost = "kodi:kodi@192.168.13.181/jsonrpc";
    std::string  sendCommandToKodi(const std::string& command, const std::string& param);
public:
    MENU_KODI(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_KODI(const MENU_KODI &base);
    MENU_KODI(MENU_KODI &&base);
    MENU_KODI &operator=(const MENU_KODI &base);
    MENU_KODI &operator=(MENU_KODI &&base);
    ~MENU_KODI();
    void entry();
    void exit();
    void keyPadOk();
    void keyPadUp();
    void keyPadDown();
    void keyPadLeft();
    void keyPadRight();
    void keyPadRes();
    void keyPadEpg();
    void keyPadPower();
    std::string getStateName();
    void timeout(std::function<void()> function = std::nullptr_t());
};

#endif // MENU_KODI_H
