#include "thermometer_container.h"

THERMOMETER_CONTAINER::THERMOMETER_CONTAINER()
{

}

void THERMOMETER_CONTAINER::add(std::string name)
{
    auto pair = std::make_pair(name, THERMOMETER(15));
    thermoMap.insert(pair);
}

void THERMOMETER_CONTAINER::setTemp(std::string name, double value)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        m->second.m_thermometer.oldTemp = m->second.m_thermometer.newTemp;
        m->second.m_thermometer.newTemp = value;
    }
    else{
        throw std::string("theromometr not found");
    }
}

double THERMOMETER_CONTAINER::getTemp(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        return m->second.m_thermometer.newTemp;
    }
    else
    {
        throw std::string("theromometr not found");
    }
}

double THERMOMETER_CONTAINER::getOldTemp(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        return m->second.m_thermometer.oldTemp;
    }
    else
    {
        throw std::string("theromometr not found");
    }
}

TEMPERATURE_STATE THERMOMETER_CONTAINER::getLastState(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        return m->second.m_thermometer.lastState;
    }
    else
    {
        throw std::string("theromometr not found");
    }
}

void THERMOMETER_CONTAINER::setState(std::string name, TEMPERATURE_STATE state)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        m->second.m_thermometer.lastState = state;
    }
    else
    {
        throw std::string("theromometr not found");
    }
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
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){

        m->second.m_stats.push_back(m->second.m_thermometer.newTemp);
    }
    else
    {
        throw std::string("theromometr not found");
    }
}

std::string THERMOMETER_CONTAINER::getStatsByName(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){

        return m->second.m_stats.stats();
    }
    else
    {
        throw std::string("theromometr not found");
    }
}

THERMOMETER::THERMOMETER(int iter):m_stats(iter)
{

}
