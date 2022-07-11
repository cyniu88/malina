#ifndef MENU_H
#define MENU_H
#include <memory>
#include <iostream>
#include "../iDom_server_OOP.h"
#include "../LCD_c/lcd_c.h"

enum class PILOT_KEY{
    KEY_POWER,
    KEY_AUDIO,
    KEY_EPG,
    KEY_OK,
    KEY_RADIO,
    KEY_TV,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_CHANNELDOWN,
    KEY_CHANNELUP,
    KEY_DOWN,
    KEY_EXIT,
    KEY_FAVORITES,
    KEY_INFO,
    KEY_LANGUAGE,
    KEY_MENU,
    KEY_MUTE,
    KEY_REFRESH,
    KEY_SAT,
    KEY_SUBTITLE,
    KEY_TEXT,
    KEY_UP,
    KEY_VOLUMEDOWN,
    KEY_VOLUMEUP,
    SLEEPER,
    DUMMY
};

enum class KEY_PAD{
    POWER = 1,
    OK = 2,
    RES = 4,
    UP = 16,
    MENU = 32,
    RIGHT = 64,
    DOWN = 256,
    LEFT = 512,
    EPG = 1024,
    REBOOT = 5,
    OFF_LCD = 777,
    TIMEOUT = 778,
};

class MENU_STATE_BASE;
class MENU_STATE_MACHINE{
public:
    std::unique_ptr<MENU_STATE_BASE> currentState;
    ~MENU_STATE_MACHINE() = default;
    void setStateMachine(std::unique_ptr<MENU_STATE_BASE> ptr);
};

class MENU_STATE_BASE
{
protected:
    thread_data* my_dataPTR;
    LCD_c* lcdPTR;
    MENU_STATE_MACHINE* stateMachinePTR;
    std::string arrow = std::string(1,(char)0) + "           " + std::string(1,(char)1);
    STATE lcdLED = STATE::OFF;
public:
    MENU_STATE_BASE(thread_data* my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED = STATE::OFF);
    MENU_STATE_BASE(const MENU_STATE_BASE& base);
    MENU_STATE_BASE(const MENU_STATE_BASE&& base);
    MENU_STATE_BASE &operator = (const MENU_STATE_BASE &base);
    MENU_STATE_BASE &operator = (MENU_STATE_BASE &&base);
    virtual ~MENU_STATE_BASE();
    virtual void entry() = 0;
    virtual void exit() = 0;
    virtual std::string getStateName() const = 0;
    virtual void keyPadPower(){std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadOk()   {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadRes()  {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadUp()   {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadDown() {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadLeft() {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadRight(){std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadMenu() {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadEpg()  {std::cout << __func__ << " pressed" << std::endl;};
    virtual void reboot()     {std::cout << __func__ << " pressed" << std::endl;};
    virtual void offLcd()     {std::cout << __func__ << " pressed" << std::endl;};
    virtual void print(const std::string &row1, const std::string &row2 = "") const{
        lcdPTR->clear();
        lcdPTR->printString(row1, row2);
    };
    virtual void timeout(std::function<void()> function = std::nullptr_t()) {};
    virtual void quickPrint(const std::string &row1, const std::string &row2 ){};

    template<class State>
    void changeStateTo(STATE lcdLED = STATE::OFF){
        this->stateMachinePTR->currentState->exit();
        auto ptr = std::make_unique<State>(my_dataPTR, lcdPTR, stateMachinePTR, lcdLED);
        this->stateMachinePTR->setStateMachine(std::move(ptr));
    }
};

class KEY_HANDLER{
protected:
    MENU_STATE_MACHINE* stateMachinePtr;
    std::mutex lock;
public:
    explicit KEY_HANDLER(MENU_STATE_MACHINE* msm);
    ~KEY_HANDLER() = default;
    void recKeyEvent(KEY_PAD eventId);
    void recIrdaEvent(PILOT_KEY eventId);
    void timeout();
    void quickPrint(const std::string &row1, const std::string &row2 );
};
#endif // MENU_H
