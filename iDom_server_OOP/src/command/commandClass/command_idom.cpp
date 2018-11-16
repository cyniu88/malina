#include "command_idom.h"
#include "../../functions/functions.h"

command_iDom::command_iDom(const std::string &name):command(name)
{
}

std::string command_iDom::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size()<2){
        return "need parameter!\n" + help();
    }
    if (v[1]=="speakers")
    {
        if (v[2] =="ON"){
            iDomTOOLS::turnOnSpeakers();
            return "speakers ON";
        }
        else if (v[2]=="OFF"){
            iDomTOOLS::turnOffSpeakers();
            return "speakers OFF";
        }
        else{
            return "unknow speakers action: "+v[2];
        }
    }
    else if (v[1]=="sunset"){
        return my_data->main_iDomTools->getSunset(true);
    }
    else if (v[1]=="sunrise"){
        return my_data->main_iDomTools->getSunrise(true);
    }
    else if (v[1]=="day" && v[2]=="lenght"){
        return my_data->main_iDomTools->getDayLenght(true);
    }
    else if (v[1]=="sun"){
        std::string ret;
        ret = my_data->main_iDomTools->getSunrise(true);
        ret.append("\n");
        ret.append(my_data->main_iDomTools->getSunset(true));
        ret.append("\n");
        ret.append(my_data->main_iDomTools->getDayLenght(true));
        ret.append("\n");
        return ret;
    }
    else if (v[1]=="sysinfo"){
        return my_data->main_iDomTools->getSystemInfo();
    }
    else if (v[1]=="temperature"){
        if ( v.size() < 3){
            return my_data->main_iDomTools->getTemperatureString();
        }
        else {
            if (v[2] == "stats"){
                std::string ret;
                try{
                 ret = my_data->main_iDomTools->getThermoStats(v[3]);
                }
                catch (std::string& obj){
                    ret = obj +" "+v[3];
                }
               return ret;
            }
        }
    }
    else if (v[1]=="text"){
        return my_data->main_iDomTools->getTextToSpeach();
    }
    else if (v[1] == "lock"){
        my_data->main_iDomTools->lockHome();
        return "hous locked";
    }
    else if (v[1] == "unlock"){
        my_data->main_iDomTools->unlockHome();
        return "hous unlocked";
    }
    else if (v[1]=="LED"){
        if (v.size() != 7){
            if (v[2]=="OFF"){
                return my_data->main_iDomTools->ledOFF();
            }
            else if(v[2] == "set"){
               return  my_data->main_iDomTools->ledOn(
                            my_data->ptr_pilot_led->colorLED[2],
                            std::stoi(v[3]),
                            std::stoi(v[4])
                            );
            }
            else{
                return "need more parameter from-to-R-G-B";
            }
        }
        else {
            LED_Strip strip(v[2],v[3],v[4],v[5],v[6]);
            return my_data->main_iDomTools->ledOn(strip);
        }
    }
    else if (v[1]=="say"){
        if (v.size() > 3){
            std::vector<std::string> vTTS ={ my_data->main_iDomTools->getTextToSpeach()};
            my_data->main_iDomTools->textToSpeach(&vTTS);
            return "sad";
        }
    }
    else if (v[1]=="smog"){
        return my_data->main_iDomTools->getSmog()+" mg/m^3";
    }
    else if (v[1]=="230V"){
        if (v.size() > 2 && v[2]=="ON"){
            my_data->main_iDomTools->turnOnPrinter();
            return "230V ON";
        }
        else if(v.size() > 2 && v[2]=="OFF"){
            my_data->main_iDomTools->turnOffPrinter();
            return "230V OFF";
        }
        else {
            return "wrong paramiter";
        }
    }
    else if (v[1]=="wifi"){
        std::string readBuffer = useful_F_libs::httpPost("http://cyniu88.no-ip.pl/cgi-bin/kto_wifi.sh",10);
        return readBuffer;
    }
    else if (v[1]=="lightning"){
        std::stringstream readBuffer;
        readBuffer << my_data->main_iDomTools->getLightningStruct().data.str();
        readBuffer << std::endl;
        readBuffer << "bool: " << my_data->main_iDomTools->getLightningStruct().riseAlarm;
        readBuffer <<std::endl <<" time: ";
        readBuffer << Clock::getTime().getString();
        return readBuffer.str();
    }
    else if (v[1]=="kill"){

        if (v[2]=="thread"){
            my_data->main_THREAD_arr->at(std::stoi(v[3]) ).thread.~thread();
            return "done!";
        }
    }
    else if (v[1]=="facebook"){
        std::string msg;
        for (unsigned int i = 2 ; i < v.size(); ++i){
            msg+=" ";
            msg+=v[i];
        }
        return my_data->main_iDomTools->postOnFacebook(msg);
    }
    else if (v[1]=="viber"){
        std::string msg;
        for (unsigned int i = 2 ; i < v.size(); ++i){
            msg+=" ";
            msg+=v[i];
        }
        STATE stMSG = my_data->main_iDomTools->sendViberMsgBool(msg, my_data->server_settings->viberReceiver.at(0),
                                                     my_data->server_settings->viberSender);
        if(stMSG == STATE::SEND_OK){
            return "wiadomosc wyslana poprawnie";
        }
        else{
            return "blad wysylania wiadomosci - sprawdz logi";
        }
    }
    else if (v[1] == "camera"){

        if (v.size() < 4){
            return "not enough parameters";
        }
        if (v[2]=="LED" && v[3] == "ON"){
            my_data->main_iDomTools->cameraLedON(my_data->server_settings->cameraLedON);
        }
        else if (v[2]=="LED" && v[3] == "OFF"){
            my_data->main_iDomTools->cameraLedOFF(my_data->server_settings->cameraLedOFF);
        }
        return "led DONE";
    }
    else if (v[1]=="weather"){

        if (v.size() < 4){
            return "not enough parameters";
        }
        std::string tempHTML = my_data->main_iDomTools->getWeatherEvent(v[2],std::stoi(v[3]));
        return useful_F_libs::removeHtmlTag(tempHTML);
    }
    else if (v[1] == "KODI"){
        return  my_data->main_iDomTools->startKodi_Thread();
    }
    else if (v[1]=="alarm"){

        if (v.size() < 3){
            return "not enough parameters";
        }
        if (v[2] == "OFF"){
            my_data->alarmTime.state = STATE::DEACTIVE;
            my_data->main_iDomStatus->setObjectState("alarm", my_data->alarmTime.state);
            my_data->main_iDomTools->saveState_iDom();
            return "alarm clock has been deactivated";
        }
        else if (v[2] == "GET"){
            return  my_data->alarmTime.time.getString();
        }
        else if (v[2] == "SET" && v.size() == 5){
            if (v[3] == "from")
            {
                my_data->alarmTime.fromVolume = std::stoi(v[4]);
            }
            else if (v[3] == "to")
            {
                my_data->alarmTime.toVolume = std::stoi(v[4]);
            }
            else if (v[3] == "radio")
            {
                my_data->alarmTime.radioID = std::stoi(v[4]);
            }
            std::stringstream ret;
            ret << "The values has beedn set:" << std::endl;
            ret << "From Value: " << my_data->alarmTime.fromVolume << std::endl;
            ret << "To Value: " << my_data->alarmTime.toVolume << std::endl;
            ret << "Radio ID: " << my_data->alarmTime.radioID << std::endl;
            my_data->main_iDomTools->saveState_iDom();
            return ret.str();
        }
        else if (v[2] == "ON" && v.size() > 3){
            my_data->alarmTime.time = Clock(v[3]);
            my_data->alarmTime.state = STATE::ACTIVE;
            my_data->main_iDomStatus->setObjectState("alarm", my_data->alarmTime.state);
            my_data->main_iDomTools->saveState_iDom();
            return "alarm clock has been activated";
        }
    }
    return "iDom - unknown parameter: "+ v[1];
}

std::string command_iDom::help()
{
    std::stringstream help;
    help << "iDom - for control smart home" << std::endl;
    help << "iDom speakers ON/OFF - to on or off speakers" << std::endl;
    help << "iDom 230v ON/OFF     - to on or off printers" << std::endl;
    help << "iDom sunset/sunrise/day lenght  - to show those parameters" << std::endl;
    help << "iDom sun        - get sunrise, sunset and day lenght" << std::endl;
    help << "iDom sysinfo    - get system info" << std::endl;
    help << "iDom text       - get text to speach" << std::endl;
    help << "iDom say <text> - say standatrd info or <text>" << std::endl;
    help << "iDom sms <text> - send sms<text>" << std::endl;
    help << "iDom LED <FROM> <TO> <R> <G> <B> - set RGB LED strip" << std::endl;
    help << "iDom LED OFF    - led off" << std::endl;
    help << "iDom LED set <from> <to> - set green led from to" << std::endl;
    help << "iDom temperature - get temperature from all termomether" << std::endl;
    help << "iDom temperature stats <name> - get temperature stats from termomether <name>" << std::endl;
    help << "iDom smog       - get current SMOG level (KRAKOW)" << std::endl;
    help << "iDom kill thread <ID>  - kill thread but denger!" << std::endl;
    help << "iDom camera LED ON/OFF - LED camera work" << std::endl;
    help << "iDom facebook ... - post on facebook wall" << std::endl;
    help << "iDom viber ...   - send viber msg" << std::endl;
    help << "iDom weather <city> <radius>  - get weather alert" << std::endl;
    help << "iDom lightning  - get lightning alert" << std::endl;
    help << "iDom alarm ON/OFF hh:mm - set larm clock" << std::endl;
    help << "iDom alarm SET from/to/radio <value> - set larm clock" << std::endl;
    help << "iDom alarm GET  - get alarm time" << std::endl;
    help << "iDom lock   - lock home" << std::endl;
    help << "iDom unlock - unlock home" << std::endl;
    help << "iDom KODI - start KODI smart TV" << std::endl;
    return help.str();
}
