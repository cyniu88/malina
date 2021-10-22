#ifndef MENU_LIGHT_H
#define MENU_LIGHT_H
#include "menu_base.h"
#include "libs/container_menu.h"

struct BULB{
    BULB(const std::string& name, int id):name(name), id(id){}
    ~BULB() = default;
    std::string name = "NULL";
    int id = 0;
};

class MENU_LIGHT : public MENU_STATE_BASE
{
    cyniu::ContainerMenuMap<std::string,cyniu::ContainerMenu<BULB,int>> lightDatabase;
public:
    MENU_LIGHT(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED = STATE::OFF);
    MENU_LIGHT(const MENU_LIGHT & base);
    MENU_LIGHT(MENU_LIGHT && base);
    MENU_LIGHT &operator= (const MENU_LIGHT& base);
    MENU_LIGHT &operator= (MENU_LIGHT &&base);
    ~MENU_LIGHT();
    void entry() override;
    void exit() override;
    std::string getStateName() override;
    void keyPadRes() override;
    void keyPadUp() override;
    void keyPadDown() override;
    void keyPadLeft() override;
    void keyPadRight() override;
    void keyPadOk() override;
    void keyPadPower() override;
    void timeout(std::function<void()> function = std::nullptr_t()) override;
};

#endif // MENU_LIGHT_H
