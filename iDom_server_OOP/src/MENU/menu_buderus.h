#ifndef MENU_BUDERUS_H
#define MENU_BUDERUS_H
#include "menu_base.h"
#include "libs/container_menu.h"

class MENU_BUDERUS : public MENU_STATE_BASE
{
    cyniu::ContainerMenu<std::string,std::function<std::string()>> menuDatabase;
public:
    MENU_BUDERUS(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_BUDERUS(const MENU_BUDERUS &base);
    MENU_BUDERUS(MENU_BUDERUS &&base);
    MENU_BUDERUS &operator=(const MENU_BUDERUS &base);
    MENU_BUDERUS &operator=(MENU_BUDERUS &&base);
    ~MENU_BUDERUS();
    void entry();
    void exit();
    void keyPadOk();
    void keyPadUp();
    void keyPadDown();
    std::string getStateName();
};

#endif // MENU_BUDERUS_H
