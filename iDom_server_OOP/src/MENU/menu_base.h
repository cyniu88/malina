#ifndef MENU_H
#define MENU_H
#include <memory>

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
    virtual void entry() = 0;
    virtual void exit() = 0;
    virtual void printStateName() = 0;
    virtual void cyniu() = 0;
};

class MENU_STATE_BASE: public MENU_STATE_BASE_IMPL
{
public:
    static std::shared_ptr<MENU_STATE_BASE_IMPL> ptr;
    MENU_STATE_BASE();
    virtual void entry() = 0;
    virtual void exit() = 0;
    virtual ~MENU_STATE_BASE() = default;
    virtual void printStateName() = 0;
    virtual void cyniu();
    template<class State>
    void changeTo(){
        ptr->exit();
        ptr = std::make_shared<State>();
        ptr->entry();
    }

};
#endif // MENU_H
