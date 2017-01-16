#include <algorithm>
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

void iDomTOOLS::sendSMSifTempChanged(std::string thermomethernName, int reference)
{
    TEMPERATURE_STATE status = hasTemperatureChange(thermomethernName,reference);
    std::string m = "temperature "+thermomethernName+" over ^"+ std::to_string(reference);

    if (status == TEMPERATURE_STATE::Over){
        my_data->myEventHandler.run("temperature")->addEvent(m);
        sendSMStoPlusGSM("yanosik-info","yanosik24","782490815",m);
    }
    else if (status == TEMPERATURE_STATE::Under){
        m ="temperature " + thermomethernName+" under \\/"+std::to_string(reference);
        my_data->myEventHandler.run("temperature")->addEvent(m);
        sendSMStoPlusGSM("yanosik-info","yanosik24","782490815",m);
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

void iDomTOOLS::textToSpeach(std::vector<std::string> *textVector)
{
    if (textVector->empty() ){
        return;
    }
    std::string txt;
    
    for (auto a : *textVector){
        txt += a;
    }
    /////////// start thread  TTS - python use ////////////////////////
    std::string command = " python /home/pi/programowanie/PYTON/gadacz.py ";
    command += "\""+ txt +"\"";
    if(my_data->ptr_MPD_info->isPlay){

    }
    else {
        turnOnSpeakers();
    }
    system(command.c_str());
    if(my_data->ptr_MPD_info->isPlay){

    }
    else {
        turnOffSpeakers();
    }
}

std::string iDomTOOLS::getTextToSpeach()
{
    std::vector<std::string> dayL = split(getDayLenght(),':');
    std::string  text;
    std::string smogText = getSmog();
    int smogInt = std::stoi(smogText);
    text =  "Godzina: "+ getTime().getString();
    text += ". \nWschód słońca: "+getSunrise();
    text += ". \nZachód słońca: "+getSunset();
    text += ". \nDługość dnia: "+ dayL[0]+" godzin "+dayL[1]+" minut";
    text +=". \n";
    dayL = getTemperature();
    dayL[1] = dayL[1].substr(0, dayL[1].size()-2);
    text += "Temperatura na zewnątrz: "+ dayL[1]+" stopnia. \n";
    text += "Temperatura w pokoju: "+ dayL[0]+" stopnia. \n";
    text += "Smog: "+ smogText +" mg/m^3. \n";
    if (smogInt > 50){
        int result = smogInt *2 ;
        text += "UWAGA! Maksymalna wartość przekroczona "+ std::to_string(result)+"%.";
    }
    return text;
}

std::vector<std::string> iDomTOOLS::getTemperature()
{
    std::vector<std::string>  vect = split(send_to_arduino(my_data,"temperature:22;"),':');
    return vect;
}

std::string iDomTOOLS::getTemperatureString()
{
    return send_to_arduino(my_data,"temperature:22;");
}

Clock iDomTOOLS::getTime()
{
    Clock t;
    time_t now = time(0);
    tm *ltm = localtime(&now);

    t.set( ltm->tm_hour ,ltm->tm_min );
    return t;
}

std::string iDomTOOLS::getSmog()
{
    return CRON::smog();
}

std::string iDomTOOLS::sendSMStoPlusGSM(std::string login, std::string pass, std::string number, std::string msg,int silentFrom , int silentTo  )
{
    if (silentFrom !=0 && silentTo !=0){
        // TODO
    }
    std::replace(msg.begin(),msg.end(),' ','+');
    std::string address = "http://darsonserver.5v.pl/bramkaPlus?login=";
    address +=login+"&password="+pass+"&sender=iDom&number="+number+"&message="+msg;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CRON::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"wysłano SMSa otreśći: " <<  msg<<std::endl;
    log_file_mutex.mutex_unlock();
    return readBuffer +"\n"+address;
}

std::string iDomTOOLS::ledOFF()
{
    return send_to_arduino(my_data,"LED_STOP:2;");
}

std::string iDomTOOLS::ledClear()
{
    return send_to_arduino(my_data,"LED_CLEAR:2;");
}

std::string iDomTOOLS::ledOn(LED_Strip ledColor)
{
    return send_to_arduino(my_data,ledColor.get());
}
