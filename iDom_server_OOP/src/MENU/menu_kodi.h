#ifndef MENU_KODI_H
#define MENU_KODI_H
#include "menu_base.h"

class MENU_KODI : public MENU_STATE_BASE
{
    int kodiVolume = 50;
    int kodiMsgId = 8;
    std::string kodiHost = "kodi:kodi@192.168.13.181/jsonrpc";
    std::string sendCommandToKodi(const std::string &command, const std::string &param);

public:
    MENU_KODI(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_KODI(const MENU_KODI &base);
    MENU_KODI(MENU_KODI &&base);
    MENU_KODI &operator=(const MENU_KODI &base);
    MENU_KODI &operator=(MENU_KODI &&base);
    ~MENU_KODI() = default;
    void entry() override;
    void exit() override;
    void keyPadOk() override;
    void keyPadUp() override;
    void keyPadDown() override;
    void keyPadLeft() override;
    void keyPadRight() override;
    void keyPadRes() override;
    void keyPadEpg() override;
    void keyPadPower() override;
    void keyPadMenu() override;
    std::string getStateName() const override;
    void timeout(std::function<void()> function = std::nullptr_t()) override;
};

#endif // MENU_KODI_H
