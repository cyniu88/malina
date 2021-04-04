#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include <vector>
#include <string>
#include "menu_base.h"
#include "libs/container_menu.h"

class MENU_MAIN : public MENU_STATE_BASE
{
    cyniu::ContainerMenu<std::string,std::function<void()>> menuDatabase;
public:
    MENU_MAIN(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED = STATE::OFF);
    MENU_MAIN(const MENU_MAIN &base);
    MENU_MAIN(MENU_MAIN &&base);
    MENU_MAIN &operator= (const MENU_MAIN & base);
    MENU_MAIN &operator= (MENU_MAIN &&base);
    ~MENU_MAIN();
    void entry() override;
    void exit() override;
    std::string getStateName() override;
    void keyPadOk() override;
    void keyPadRes() override;
    void keyPadUp() override;
    void keyPadDown() override;
};

#endif // MENU_MAIN_H
