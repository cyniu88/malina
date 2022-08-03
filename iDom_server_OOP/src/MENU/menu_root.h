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
    void entry() override;
    void exit() override;
    std::string getStateName() const override;
    void keyPadMenu() override;
    void keyPadPower() override;
    void keyPadUp() override;
    void keyPadDown() override;
    void keyPadLeft() override;
    void keyPadRight() override;
    void keyPadEpg() override;
    void keyPadRes() override;
    void reboot() override;
    void timeout(std::function<void()> function = std::nullptr_t()) override;
    void offLcd() override;
    void quickPrint(const std::string &row1, const std::string & row2) override;
private:
    void volumeUp();
    void volumeDown();
    void mpdStartStop();
    void mpdNext();
    void mpdPrev();
    int tempCounter;
    const unsigned int numberOfChar = 16;
    int numberOfCharCounter = 0;
    void scrollText() override;
    unsigned int time = 0;
};

#endif // MENU_ROOT_H
