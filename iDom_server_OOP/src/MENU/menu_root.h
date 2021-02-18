#ifndef MENU_ROOT_H
#define MENU_ROOT_H

#include "menu_base.h"

class MENU_ROOT : public MENU_STATE_BASE
{
public:
    MENU_ROOT(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED = STATE::OFF);
    MENU_ROOT(const MENU_ROOT &base);
    MENU_ROOT(MENU_ROOT &&base);
    MENU_ROOT &operator= (const MENU_ROOT &base);
    MENU_ROOT &operator= (MENU_ROOT &&base);
    ~MENU_ROOT();
    void entry();
    void exit();
    std::string getStateName();
    void keyPadMenu();
    void keyPadPower();
    void keyPadUp();
    void keyPadDown();
    void keyPadLeft();
    void keyPadRight();
    void keyPadEpg();
    void reboot();
    void timeout(std::function<void()> function = std::nullptr_t());
    void offLcd();
    void quickPrint(const std::string &row1, const std::string & row2);
private:
    void volumeUp();
    void volumeDown();
    void mpdStartStop();
    void mpdNext();
    void mpdPrev();
    int tempCounter;
};

#endif // MENU_ROOT_H
