#include "idomtools.h"

iDomTOOLS::iDomTOOLS(thread_data *myData)
{
    my_data = myData;
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
    if (cur->second.newTemp >= reference && cur->second.oldTemp < reference){
        return TEMPERATURE_STATE::Over;
    }
    else if (cur->second.newTemp <= reference && cur->second.oldTemp > reference )
    {
        return TEMPERATURE_STATE::Under;
    }
    else
    {
        return TEMPERATURE_STATE::NoChanges;
    }
    return TEMPERATURE_STATE::Unknown;
}

void iDomTOOLS::sendSMSifTempChanged(std::string thermomethernName, int reference, std::string phoneNumber, std::string msg)
{
    TEMPERATURE_STATE status = hasTemperatureChange(thermomethernName,reference);

    if (status == TEMPERATURE_STATE::Over){
        my_data->myEventHandler.run("temperature")->addEvent("temperature "+thermomethernName+"\tover "+ std::to_string(reference));
    }
    else if (status == TEMPERATURE_STATE::Under){
        my_data->myEventHandler.run("temperature")->addEvent("temperature " + thermomethernName+"\tunder "+std::to_string(reference));
    }
    else{
        //my_data->myEventHandler.run("unknown")->addEvent("temperatura nie przeszla przez "+std::to_string(reference));
    }
}

void iDomTOOLS::turnOnSpeakers()
{
    digitalWrite(GPIO_SPIK, LOW);
}

void iDomTOOLS::turnOffSpeakers()
{
    digitalWrite(GPIO_SPIK, HIGH);
}
