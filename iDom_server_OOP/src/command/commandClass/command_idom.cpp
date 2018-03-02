#include "command_idom.h"
#include "../../functions/functions.h"

command_iDom::command_iDom(std::string name):command(name)
{

}

std::string command_iDom::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size()<2){
        return "need parameter!";
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
                catch (std::string obj){
                    ret = obj +" "+v[3];
                }
               return ret;
            }
        }
    }
    else if (v[1]=="sms"){
        if (v.size() <3){
            return "function not ready yet";
        }
        else{
            std::string m;
            for(size_t i = 2; i<v.size();++i){
                m+= v[i]+" ";
            }
            return my_data->main_iDomTools->sendSMStoPlusGSM("yanosik-info","yanosik24","782490815",m);
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
        std::string readBuffer = useful_F::httpPost("http://cyniu88.no-ip.pl/cgi-bin/kto_wifi.sh",10);
        return readBuffer;
    }
    else if (v[1]=="kill"){

        if (v[2]=="thread"){
            my_data->main_THREAD_arr[std::stoi(v[3]) ].thread.~thread();
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
        return my_data->main_iDomTools->sendViberMsg(msg, my_data->server_settings->viberReceiver.at(0),
                                                     my_data->server_settings->viberSender);
    }
    else if (v[1]=="camera"){

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
        return my_data->main_iDomTools->getWeatherEvent(v[2],std::stoi(v[3]));
    }
    else if (v[1]=="alarm"){

        if (v.size() < 3){
            return "not enough parameters";
        }
        if (v[2] == "OFF"){
            my_data->alarmTime.state = STATE::DEACTIVE;
            my_data->main_iDomStatus->setObjectState("alarm", my_data->alarmTime.state);
            return "alarm clock has been deactivated";
        }

        else if (v[2] == "ON" && v.size() > 3){
            my_data->alarmTime.time = Clock(v[3]);
            my_data->alarmTime.state = STATE::ACTIVE;
            my_data->main_iDomStatus->setObjectState("alarm", my_data->alarmTime.state);
            return "alarm clock has been activated";
        }
    }
    return "iDom - unknown parameter: "+ v[1];
}

std::string command_iDom::help()
{
    std::string ret = "iDom - for control smart home\n";
    ret.append("iDom speakers ON/OFF - to on or off speakers\n");
    ret.append("iDom 230v ON/OFF     - to on or off printers");
    ret.append("iDom sunset/sunrise/day lenght  - to show those parameters\n");
    ret.append("iDom sun        - get sunrise, sunset and day lenght\n");
    ret.append("iDom sysinfo    - get system info \n");
    ret.append("iDom text       - get text to speach\n");
    ret.append("iDom say <text> - say standatrd info or <text> \n");
    ret.append("iDom sms <text> - send sms<text> \n");
    ret.append("iDom LED <FROM> <TO> <R> <G> <B> - set RGB LED strip\n");
    ret.append("iDom LED OFF    - led off\n");
    ret.append("iDom LED set <from> <to> - set green led from to\n");
    ret.append("iDom temperature - get temperature from all termomether\n");
    ret.append("iDom temperature stats <name> - get temperature stats from termomether <name>\n");
    ret.append("iDom smog       - get current SMOG level (KRAKOW)\n");
    ret.append("iDom kill thread <ID>  - kill thread but denger!\n");
    ret.append("iDom camera LED ON/OFF - LED camera work\n");
    ret.append("iDom facebook ... - post on facebook wall\n");
    ret.append("iDom viber ...   - send viber msg\n");
    ret.append("iDom weather <city> <radius>  - get weather alert\n");
    ret.append("iDom alarm ON/OFF hh:mm - set larm clock \n");
    ret.append("iDom lock   - lock home\n");
    ret.append("iDom unlock - unlock home\n");
    return ret;
}
