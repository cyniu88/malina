#ifndef IDOMTOOLS_H
#define IDOMTOOLS_H

#include <map>

enum class TEMPERATURE_STATE{
    Under ,
    Over,
    NoChanges,
    Unknown
};
struct temperature {
    float newTemp = 0;
    float oldTemp = 0;
};

class iDomTOOLS
{
 std::map <std::string ,temperature> thermometer;

public:
    iDomTOOLS();

    void setTemperature (std::string name, int value);
    TEMPERATURE_STATE hasTemperatureChange(std::string thermometerName, int reference);
};

#endif // IDOMTOOLS_H
