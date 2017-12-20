#ifndef THERMOMETER_CONTAINER_H
#define THERMOMETER_CONTAINER_H
#include <map>
#include <vector>

enum class TEMPERATURE_STATE{
    Under,
    Over,
    NoChanges,
    Unknown
};

struct temperature {
    //temperature(std::string name) : thermometrName(name){};
    double newTemp = 1.0101;
    double oldTemp = 1.0101;
    double maxValue = 0;
    std::string maxDate;
    double minValue = 0;
    std::string minDate;
    std::string thermometrName;

    TEMPERATURE_STATE lastState = TEMPERATURE_STATE::Unknown;
};
class THERMOMETER {
public:
    temperature t;
};

class THERMOMETER_CONTAINER
{
    std::map<std::string,THERMOMETER>  thermoMap;
public:
    THERMOMETER_CONTAINER();
    void add(std::string name);
    void setTemp(std::string name, double value);
    double getTemp(std::string name);
    double getOldTemp(std::string name);
    TEMPERATURE_STATE getLastState(std::string name);
    void setState(std::string name, TEMPERATURE_STATE state);
    void updateAll(std::vector<std::string>* vectorThermo);
};

#endif // THERMOMETER_CONTAINER_H
