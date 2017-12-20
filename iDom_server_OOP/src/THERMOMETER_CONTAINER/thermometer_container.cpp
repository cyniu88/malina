#include "thermometer_container.h"

THERMOMETER_CONTAINER::THERMOMETER_CONTAINER()
{

}

void THERMOMETER_CONTAINER::add(std::string name)
{
    auto pair = std::make_pair(name, THERMOMETER());
    thermoMap.insert(pair);
}

void THERMOMETER_CONTAINER::setTemp(std::string name, double value)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        m->second.t.oldTemp = m->second.t.newTemp;
        m->second.t.newTemp = value;
    }
    else{
        throw "theromometr not found";
    }
}

double THERMOMETER_CONTAINER::getTemp(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        return m->second.t.newTemp;
    }
    else
    {
        throw "theromometr not found";
    }
}

double THERMOMETER_CONTAINER::getOldTemp(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        return m->second.t.oldTemp;
    }
    else
    {
        throw "theromometr not found";
    }
}

TEMPERATURE_STATE THERMOMETER_CONTAINER::getLastState(std::string name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
        return m->second.t.lastState;
    }
    else
    {
        throw "theromometr not found";
    }
}

void THERMOMETER_CONTAINER::setState(std::string name, TEMPERATURE_STATE state)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end()){
       m->second.t.lastState = state;
    }
    else
    {
        throw "theromometr not found";
    }
}

void THERMOMETER_CONTAINER::updateAll(std::vector<std::string> *vectorThermo)
{
    std::string in  = vectorThermo->at(0);
    std::string out = vectorThermo->at(1);
    setTemp("inside", std::stod(in));
    setTemp("outside",std::stod(out));
}
