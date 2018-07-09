#include "thermometer_container.h"

THERMOMETER *THERMOMETER_CONTAINER::returnThermometerPtr(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end())
    {
        return &(m->second);
    }
    else
    {
        throw std::string("thermometer not found!");
    }
}

THERMOMETER_CONTAINER::THERMOMETER_CONTAINER()
{
}

THERMOMETER::THERMOMETER(int iter):m_stats(iter)
{
}

void THERMOMETER_CONTAINER::add(std::string name)
{
    auto pair = std::make_pair(name, THERMOMETER(15));
    thermoMap.insert(pair);
}

void THERMOMETER_CONTAINER::setTemp(std::string name, double value)
{
    returnThermometerPtr(name)->m_thermometer.oldTemp = returnThermometerPtr(name)->m_thermometer.newTemp;
    returnThermometerPtr(name)->m_thermometer.newTemp = value;
}

double THERMOMETER_CONTAINER::getTemp(std::string name)
{
    return returnThermometerPtr(name)->m_thermometer.newTemp;
}

double THERMOMETER_CONTAINER::getOldTemp(std::string name)
{
    return returnThermometerPtr(name)->m_thermometer.oldTemp;
}

TEMPERATURE_STATE THERMOMETER_CONTAINER::getLastState(std::string name)
{
    return returnThermometerPtr(name)->m_thermometer.lastState;
}

void THERMOMETER_CONTAINER::setState(std::string name, TEMPERATURE_STATE state)
{
    returnThermometerPtr(name)->m_thermometer.lastState = state;
}

void THERMOMETER_CONTAINER::updateAll(std::vector<std::string> *vectorThermo)
{
    std::string in  = vectorThermo->at(0);
    std::string out = vectorThermo->at(1);
    setTemp("inside", std::stod(in));
    setTemp("outside",std::stod(out));
}

void THERMOMETER_CONTAINER::updateStats(std::string name)
{
    returnThermometerPtr(name)->m_stats.push_back(returnThermometerPtr(name)->m_thermometer.newTemp);
}

std::string THERMOMETER_CONTAINER::getStatsByName(std::string name)
{
    return returnThermometerPtr(name)->m_stats.stats();
}

bool THERMOMETER_CONTAINER::isMoreDiff(std::string name, double diff)
{
    return returnThermometerPtr(name)->m_stats.isMoreDiff(diff);
}

std::pair<double, double> THERMOMETER_CONTAINER::getLast2(std::string name)
{
    return returnThermometerPtr(name)->m_stats.getLast2();
}
