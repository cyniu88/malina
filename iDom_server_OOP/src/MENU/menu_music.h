#pragma once

#include "menu_base.h"

class MENU_MUSIC : public MENU_STATE_BASE
{
public:
    MENU_MUSIC(thread_context *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_MUSIC(const MENU_MUSIC &base);
    MENU_MUSIC(MENU_MUSIC &&base);
    MENU_MUSIC &operator=(const MENU_MUSIC &base);
    MENU_MUSIC &operator=(MENU_MUSIC &&base);
    ~MENU_MUSIC() = default;
    void entry() override;
    void exit() override;
    void keyPadPower() override;
    void keyPadUp() override;
    void keyPadDown() override;
    void keyPadLeft() override;
    void keyPadRight() override;
    std::string getStateName() const override;
};

