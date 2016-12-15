#include "idomtools.h"

iDomTOOLS::iDomTOOLS()
{
    thermometer["inside"];
    thermometer["outside"];
}

void iDomTOOLS::setTemperature(std::string name, float value)
{
     auto cur = thermometer.find(name);
     cur->second.oldTemp = cur->second.newTemp ;
     cur->second.newTemp = value;
}

TEMPERATURE_STATE iDomTOOLS::hasTemperatureChange(std::string thermometerName,int reference )
{
    auto cur = thermometer.find(thermometerName);
    if (cur->second.newTemp > reference && cur->second.oldTemp < reference){
        return TEMPERATURE_STATE::Over;
    }
    else if (cur->second.newTemp < reference && cur->second.oldTemp > reference )
    {
        return TEMPERATURE_STATE::Under;
    }
    else
    {
        return TEMPERATURE_STATE::NoChanges;
    }
    return TEMPERATURE_STATE::Unknown;

}


