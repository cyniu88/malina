#ifndef MENU_INFO_H
#define MENU_INFO_H

#include <vector>

#include "menu_base.h"

class MENU_INFO : public MENU_STATE_BASE
{
public:
    MENU_INFO(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED = STATE::OFF);
    MENU_INFO(const MENU_INFO &base);
    MENU_INFO(MENU_INFO &&base);
    MENU_INFO &operator= (const MENU_INFO &base);
    MENU_INFO &operator= (MENU_INFO &&base);
    ~MENU_INFO();
    void entry() override;
    void exit() override;
    std::string getStateName() const override;
    void keyPadUp() override;
    void keyPadDown() override;
    void keyPadRes() override;
    void timeout(std::function<void()> function = std::nullptr_t()) override;
private:
    std::vector<nlohmann::json> m_lightData;
};

#endif // MENU_INFO_H
