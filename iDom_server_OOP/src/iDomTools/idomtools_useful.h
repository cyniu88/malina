#ifndef IDOMTOOLS_USEFULL_H
#define IDOMTOOLS_USEFULL_H
#include <string>

enum class PIN_STATE{
    LOW_STATE = 0,
    HIGH_STATE = 1,
    UNKNOWN_STATE = 2
};

enum class USER_LEVEL{
    ROOT,
    USER
};

struct WEATHER_ALER{
    std::string name;
    std::string alert;
    bool sended = false;
};

#endif // IDOMTOOLS_USEFULL_H
