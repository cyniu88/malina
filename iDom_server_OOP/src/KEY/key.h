#ifndef KEY_H
#define KEY_H
#include <iostream>
#include <map>
#include <utility>      // std::pair
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

class KEY
{
public:
    KEY(PILOT_KEY value, const std::string& name) ;
    virtual ~KEY() ;
    std::string getName() const;
    virtual  PILOT_KEY getValue() const;

protected:
    PILOT_KEY _value;
    std::string key_name;
};

class SuperKEY : public KEY
{
public:
    SuperKEY (PILOT_KEY v, const std::string &n, std::string LogName) ;
    virtual ~SuperKEY() ;
    PILOT_KEY getValue() const;

protected:
    std::string LogName;
};

class pilot
{
    std::map <std::string , std::unique_ptr <KEY>  > *key_map;
public:
    pilot(std::map <std::string , std::unique_ptr <KEY>  > *key_map) ;
    virtual ~pilot( );
    void setup();
};

#endif // KEY_H
