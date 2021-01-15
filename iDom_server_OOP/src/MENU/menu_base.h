#ifndef MENU_H
#define MENU_H
#include <memory>
#include <iostream>
#include "../iDom_server_OOP.h"

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
    DOWN = 265,
    LEFT = 512,
    EPG = 1024,
};
class MENU_STATE_BASE_IMPL{
public:
    virtual ~MENU_STATE_BASE_IMPL() = default;
    virtual void entry() = 0;
    virtual void exit() = 0;
    virtual std::string getStateName() = 0;
    virtual void keyPadPower() = 0;
    virtual void keyPadOk() = 0;
    virtual void keyPadRes() = 0;
    virtual void keyPadUp() = 0;
    virtual void keyPadDown() = 0;
    virtual void keyPadLeft() = 0;
    virtual void keyPadRight() = 0;
    virtual void keyPadMenu() = 0;
    virtual void keyPadEpg() = 0;
};

class MENU_STATE_BASE: public MENU_STATE_BASE_IMPL
{
protected:
    thread_data* my_dataPTR;
public:
    static std::unique_ptr<MENU_STATE_BASE_IMPL> ptr;
    MENU_STATE_BASE(thread_data* my_data);
    virtual void entry() = 0;
    virtual void exit() = 0;
    virtual ~MENU_STATE_BASE() = default;
    virtual std::string getStateName() = 0;
    virtual void keyPadPower(){std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadOk()   {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadRes()  {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadUp()   {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadDown() {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadLeft() {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadRight(){std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadMenu() {std::cout << __func__ << " pressed" << std::endl;};
    virtual void keyPadEpg()  {std::cout << __func__ << " pressed" << std::endl;};


    template<class State>
    void changeTo(){
        ptr->exit();
        ptr = std::make_unique<State>(my_dataPTR);
        ptr->entry();
    }

};

class KEY_HANDLER{
public:
    KEY_HANDLER(thread_data *my_data);
    void recKeyEvent(KEY_PAD eventId);
};

#endif // MENU_H
