#ifndef IDOMTOOLS_H
#define IDOMTOOLS_H

#include <map>
#include <string>
#include "../iDom_server_OOP.h"
#include "../../libs/sunrise-sunset/sunriseset.h"
struct thread_data;

enum class TEMPERATURE_STATE{
    Under ,
    Over,
    NoChanges,
    Unknown
};
struct temperature {
    float newTemp = 0.0001;
    float oldTemp = 0.0001;
};

class iDomTOOLS
{
    std::map <std::string ,temperature> thermometer;
    thread_data *my_data;
    SunRiseSet sun;
public:
    iDomTOOLS(thread_data *myData);

    void setTemperature (std::string name, float value);
    TEMPERATURE_STATE hasTemperatureChange(std::string thermometerName, int reference);
    void sendSMSifTempChanged(std::string thermomethernName, int reference, std::string phoneNumber, std::string msg );
    void turnOnSpeakers();
    void turnOffSpeakers();
    std::string getSunrise();
    std::string getSunset();
    std::string getDayLenght();
};

#endif // IDOMTOOLS_H
