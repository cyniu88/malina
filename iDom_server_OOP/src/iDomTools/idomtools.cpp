#include "idomtools.h"
#include "../functions/functions.h"
#include "../CRON/cron.hpp"
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
    reference+=0.0002;
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

std::string iDomTOOLS::getSunrise(bool extend )
{
    Clock tt = sun.getSunRise();
    if (extend == true){
        return "Sunrise time: "+tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getSunset(bool extend )
{
    Clock tt = sun.getSunSet();
    if (extend == true){
        return  "Sunset time: "+tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getDayLenght(bool extend )
{
    Clock tt = sun.getDayLength();
    if (extend == true){
        return "Day Lenght : "+tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getSystemInfo()
{
    struct sysinfo info;
    sysinfo(&info);
    long input_seconds = info.uptime;
    auto days = input_seconds / 60 / 60 / 24;
    auto hours = (input_seconds / 60 / 60) % 24;
    auto minutes = (input_seconds / 60) % 60;
    auto seconds = input_seconds % 60;
    
    std::string  ret;
    ret=  "System uptime: " + std::to_string(days)+" day ";
    ret+=  std::to_string(hours) +" hours " ;
    ret+=  std::to_string(minutes)+ " minutes ";
    ret+=  std::to_string(seconds)+ " seconds ";
    ret.append("\n");
    ret+= std::to_string(info.freeram);
    return ret;
}

void iDomTOOLS::textToSpeach(std::vector<std::string> *textVector) const
{
    if (textVector->empty() ){
        return;
    }
    std::string txt;
    
    for (auto a : *textVector){
        txt += a;
    }
    /////////// start thread  TTS - python use ////////////////////////
    
}

std::string iDomTOOLS::getTextToSpeach()
{
    auto start = std::chrono::system_clock::now();
    std::vector<std::string> dayL = split(getDayLenght(),':');
    std::string  text;
    text =  "Godzina:";
    text += ". \nWschód słońca: "+getSunrise();
    text += ". \nZachód słońca: "+getSunset();
    text += ". \nDługość dnia: "+ dayL[0]+" godzin "+dayL[1]+" minut";
    text +=". \n";
    dayL = getTemperature();
    text += "Temperatura na zewnątrz:"+ dayL[1]+" stopnia. \n";
    text += "Temperatura w pokoju:"+ dayL[0]+" stopnia. \n";
    text += "Smog: "+  CRON::smog()+"miligram na metr szescienny. \n";
    return text;
}

std::vector<std::string> iDomTOOLS::getTemperature()
{
   std::vector<std::string>  vect = split(send_to_arduino(my_data,"temperature:22;"),':');
   return vect;
}
