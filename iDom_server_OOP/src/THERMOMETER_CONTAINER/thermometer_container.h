#ifndef THERMOMETER_CONTAINER_H
#define THERMOMETER_CONTAINER_H
#include <map>
#include <vector>
#include "../../libs/Statistic/statistic.h"

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
    THERMOMETER(int iter);

    temperature m_thermometer;
    STATISTIC<double> m_stats;
};

class THERMOMETER_CONTAINER
{
    std::map<std::string,THERMOMETER>  thermoMap;
    THERMOMETER *returnThermometerPtr(std::string name);
public:
    THERMOMETER_CONTAINER();
    void add(std::string name);
    void setTemp(std::string name, double value);
    double getTemp(const std::string& name);
    double getOldTemp(std::string name);
    TEMPERATURE_STATE getLastState(std::string name);
    void setState(std::string name, TEMPERATURE_STATE state);
    void updateAll(std::vector<std::string>* vectorThermo);
    void updateStats(std::string name);
    std::string getStatsByName(std::string name);

    bool isMoreDiff(std::string name, double diff);
    std::pair<double,double> getLast2(std::string name);
};

#endif // THERMOMETER_CONTAINER_H
