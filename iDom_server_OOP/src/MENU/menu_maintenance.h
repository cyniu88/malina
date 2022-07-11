#ifndef MENU_MAINTENANCE_H
#define MENU_MAINTENANCE_H

#include "menu_base.h"
#include "libs/container_menu.h"

class MENU_MAINTENANCE : public MENU_STATE_BASE
{
    cyniu::ContainerMenu<std::string,std::function<void()>> menuDatabase;
public:
    MENU_MAINTENANCE(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED);
    ~MENU_MAINTENANCE() = default;
    void entry() override;
    void exit() override;
    std::string getStateName() const override;
    void keyPadOk() override;
    void keyPadRes() override;
    void keyPadUp() override;
    void keyPadDown() override;
    void timeout(std::function<void()> function = std::nullptr_t()) override;
};

#endif // MENU_MAINTENANCE_H
