#ifndef MENU_VOLUME_H
#define MENU_VOLUME_H
#include "menu_base.h"

class MENU_VOLUME : public MENU_STATE_BASE
{
public:
    MENU_VOLUME(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED = STATE::OFF);
    MENU_VOLUME(const MENU_VOLUME &base);
    MENU_VOLUME(MENU_VOLUME &&base);
    MENU_VOLUME &operator=(const MENU_VOLUME &base);
    MENU_VOLUME &operator=(MENU_VOLUME &&base);
    ~MENU_VOLUME() = default;
    void entry() override;
    void exit() override;
    std::string getStateName() const override;
};

#endif // MENU_VOLUME_H
