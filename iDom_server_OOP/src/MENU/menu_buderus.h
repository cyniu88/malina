#pragma once

#include "menu_base.h"
#include "libs/container_menu.h"

class MENU_BUDERUS : public MENU_STATE_BASE
{
    cyniu::ContainerMenu<std::string, std::function<std::string()>> menuDatabase;

public:
    MENU_BUDERUS(thread_context *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_BUDERUS(const MENU_BUDERUS &base);
    MENU_BUDERUS(MENU_BUDERUS &&base);
    MENU_BUDERUS &operator=(const MENU_BUDERUS &base);
    MENU_BUDERUS &operator=(MENU_BUDERUS &&base);
    ~MENU_BUDERUS() = default;
    void entry() override;
    void exit() override;
    void keyPadOk() override;
    void keyPadUp() override;
    void keyPadDown() override;
    void keyPadRes() override;
    std::string getStateName() const override;
};

