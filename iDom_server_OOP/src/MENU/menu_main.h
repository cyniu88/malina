#pragma once

#include <vector>
#include <string>
#include "menu_base.h"
#include "libs/container_menu.h"

class MENU_MAIN : public MENU_STATE_BASE
{
    cyniu::ContainerMenu<std::string, std::function<void()>> menuDatabase;

public:
    MENU_MAIN(thread_context *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_MAIN(const MENU_MAIN &base);
    MENU_MAIN(MENU_MAIN &&base);
    MENU_MAIN &operator=(const MENU_MAIN &base);
    MENU_MAIN &operator=(MENU_MAIN &&base);
    ~MENU_MAIN() = default;
    void entry() override;
    void exit() override;
    std::string getStateName() const override;
    void keyPadOk() override;
    void keyPadRes() override;
    void keyPadUp() override;
    void keyPadDown() override;
    void timeout(std::function<void()> function = std::nullptr_t()) override;
};

