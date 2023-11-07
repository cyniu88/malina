#include "command_idom.h"
#include "../../iDomTools/idomtools_interface.h"

command_iDom::command_iDom(const std::string &name):command(name)
{
}

std::string command_iDom::execute(std::vector<std::string> &v, thread_data *context)
{
    if (v.size()<2){
        return "need parameter!\n" + help();
    }
    if (v[1] == "speakers")
    {
        if (v[2] == "ON"){
            context->main_iDomTools->turnOnSpeakers();
            return "speakers ON";
        }
        else if (v[2] == "OFF"){
            context->main_iDomTools->turnOffSpeakers();
            return "speakers OFF";
        }
        else{
            return "unknow speakers action: "+v[2];
        }
    }
    else if (v[1] == "sunset"){
        return context->main_iDomTools->getSunset(true);
    }
    else if (v[1] == "sunrise"){
        return context->main_iDomTools->getSunrise(true);
    }
    else if (v[1] == "day" && v[2] == "lenght"){
        return context->main_iDomTools->getDayLenght(true);
    }
    else if (v[1] == "sun"){
        std::string ret;
        ret = context->main_iDomTools->getSunrise(true);
        ret.append("\n");
        ret.append(context->main_iDomTools->getSunset(true));
        ret.append("\n");
        ret.append(context->main_iDomTools->getDayLenght(true));
        ret.append("\n");
        return ret;
    }
    else if (v[1] == "sysinfo"){
        return context->main_iDomTools->getSystemInfo();
    }
    else if (v[1] == "lusina") //TODO temorary
    {
        std::stringstream ret;
        ret << context->lusina.statHumi.stats() <<std::endl;
        ret << context->lusina.statTemp.stats();
        return ret.str();
    }
    else if (v[1] == "temperature"){
        if(v.size() < 3){
            return context->main_iDomTools->getTemperatureString();
        }
        else {
            if (v[2] == "stats"){
                std::string ret;
                try{
                    ret = context->main_iDomTools->getThermoStats(v[3]);
                }
                catch (std::string& obj){
                    ret = obj + " " + v[3];
                }
                return ret;
            }
        }
    }
    else if (v[1] == "text"){
        return context->main_iDomTools->getTextToSpeach();
    }
    else if (v[1] == "lock"){
        context->main_iDomTools->lockHome();
        return "house locked";
    }
    else if (v[1] == "unlock"){
        context->main_iDomTools->unlockHome();
        return "house unlocked";
    }
    else if (v[1] == "say"){
        if (v.size() > 3){
            std::vector<std::string> vTTS = { context->main_iDomTools->getTextToSpeach()};
            context->main_iDomTools->textToSpeach(&vTTS);
            return "sad";
        }
    }
    else if (v[1] == "smog"){
        return context->main_iDomTools->getSmog() + " mg/m^3";
    }
    else if (v[1] == "230V"){
        if (v.size() > 2 && v[2] == "ON"){
            //context->main_iDomTools->turnOnPrinter();
            return "230V ON";
        }
        else if(v.size() > 2 && v[2] == "OFF"){
            //context->main_iDomTools->turnOffPrinter();
            return "230V OFF";
        }
        else if(v.size() > 2 && v[2] == "OUTDOOR"){
            context->main_iDomTools->turnOnOff230vOutdoor();
            return "change outdoor";
        }
        else {
            return "wrong paramiter";
        }
    }
    else if (v[1] == "wifi"){
        std::string readBuffer = useful_F_libs::httpPost("http://45.90.3.84/cgi-bin/kto_wifi.sh",10);
        return readBuffer;
    }

    else if (v[1] == "kill"){

        if (v[2]=="thread"){
            context->main_THREAD_arr->at(std::stoul(v[3]) ).thread.~thread();
            return "done!";
        }
    }
    else if (v[1] == "facebook"){
        std::string msg;
        for (unsigned int i = 2; i < v.size(); ++i){
            msg.push_back(' ');
            msg.append(v[i]);
        }
        return context->main_iDomTools->postOnFacebook(msg);
    }
    else if (v[1] == "viber"){
        std::string msg;
        for (unsigned int i = 2; i < v.size(); ++i){
            msg.push_back(' ');
            msg.append(v[i]);
        }
        STATE stMSG = context->main_iDomTools->sendViberMsgBool(msg, context->server_settings->_fb_viber.viberReceiver.at(0),
                                                                context->server_settings->_fb_viber.viberSender);
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
        if (v[2] == "LED" && v[3] == "ON"){
            context->main_iDomTools->cameraLedON(context->server_settings->_camera.cameraLedON);
        }
        else if (v[2] == "LED" && v[3] == "OFF"){
            context->main_iDomTools->cameraLedOFF(context->server_settings->_camera.cameraLedOFF);
        }
        return "led DONE";
    }
    else if (v[1] == "weather"){

        if (v.size() < 4){
            return "not enough parameters";
        }
        std::string tempHTML = context->main_iDomTools->getWeatherEvent(v[2],std::stoi(v[3]));
        return useful_F_libs::removeHtmlTag(tempHTML);
    }
    else if (v[1] == "KODI"){
        return context->main_iDomTools->startKodi_Thread();
    }
    else if (v[1] == "health") {
        return context->iDomAlarm.showAlarm();
    }
    else if (v[1] == "isDay"){
        return context->main_iDomTools->isItDay() ? "isDay:true" : "isDay:false";
    }
    else if (v[1] == "blink"  and
             context->server_settings->_runThread.SATEL == true) {
        context->satelIntegraHandler->getSatelPTR()->outputOn(context->server_settings->_satel_integra.outdoor_siren_lights_id); //turn on satel output to blink outdoor siren
        return "done " + std::to_string(counter);
    }
    else if (v[1] == "doorbell") {
        // context->main_iDomTools->sendViberPicture("DZWONEK do bramy!",
        //                                           "https://png.pngtree.com/element_our/20190529/ourmid/pngtree-ring-the-doorbell-icon-image_1198163.jpg",
        //                                           context->server_settings->_fb_viber.viberReceiver.at(0),
        //                                           context->server_settings->_fb_viber.viberSender);   // inform  door bell has been pressed
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Dzwonek do bramy"<< std::endl;
        log_file_mutex.mutex_unlock();
        //TODO add doorbell actions if needed
        context->main_house_room_handler->turnOnAllInRoom("dzwonek");
        if(context->satelIntegraHandler != std::nullptr_t()){
            context->satelIntegraHandler->getSatelPTR()->outputOn(context->server_settings->_satel_integra.outdoor_siren_lights_id); //turn on satel output to blink outdoor siren in case of gate doorbell
        }
        else{
            log_file_mutex.mutex_lock();
            log_file_cout << WARNING << "Satel gateway not started!"<< std::endl;
            log_file_mutex.mutex_unlock();
        }
        return "done " + std::to_string(counter);
    }
    else if (v[1] == "alarm"){

        if (v.size() < 3){
            return "not enough parameters";
        }
        if (v[2] == "OFF"){
            context->alarmTime.state = STATE::DEACTIVE;
            context->main_Rs232->print("LED_CLOCK:0;");
            context->main_iDomStatus->setObjectState("alarm", context->alarmTime.state);
            context->main_iDomTools->saveState_iDom(context->serverStarted);
            context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                          stateToString(STATE::DEACTIVE));
            return "alarm clock has been deactivated";
        }
        else if (v[2] == "GET"){
            return context->alarmTime.time.getString();
        }
        else if (v[2] == "SET" && v.size() == 5){
            if (v[3] == "from")
            {
                context->alarmTime.fromVolume = std::stoi(v[4]);
            }
            else if (v[3] == "to")
            {
                context->alarmTime.toVolume = std::stoi(v[4]);
            }
            else if (v[3] == "radio")
            {
                context->alarmTime.radioID = std::stoi(v[4]);
            }
            else if (v[3] == "command")
            {
                context->alarmTime.commands.push_back("light room pokoj_N on");
                context->alarmTime.commands.push_back("light room korytarz on");
            }
            std::stringstream ret;
            ret << "The values has beedn set:" << std::endl;
            ret << "From Value: " << context->alarmTime.fromVolume << std::endl;
            ret << "To Value: " << context->alarmTime.toVolume << std::endl;
            ret << "Radio ID: " << context->alarmTime.radioID << std::endl;
            context->main_iDomTools->saveState_iDom(context->serverStarted);
            return ret.str();
        }
        else if (v[2] == "ON" && v.size() > 3){
            context->alarmTime.time = Clock(v[3]);
            context->alarmTime.state = STATE::ACTIVE;
            context->main_Rs232->print("LED_CLOCK:1;");
            context->main_iDomStatus->setObjectState("alarm", context->alarmTime.state);
            context->main_iDomTools->saveState_iDom(context->serverStarted);
            context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                          stateToString(STATE::ACTIVE));
            return "alarm clock has been activated";
        }

    }

    else if (v[1] == "key" && v[2] == "add")
    {
        if(v[3] == "tmp")
            context->m_keyHandler->addKEY(v[4],"null", std::stoul(v[5]),true);
        else {
            context->m_keyHandler->addKEY(v[3],"null", std::stoul(v[4]),false);
        }
        return context->m_keyHandler->listKEY();
    }
    else if (v[1] == "key" && v[2] == "remove" && v.size() == 4)
    {
        context->m_keyHandler->removeKEY(v[3]);
        return v[3] + " removed";
    }
    else if(v[1] == "link" && v.size() > 2)
    {
        v.erase(v.begin(), v.begin() + 2);
        return context->main_iDomTools->getLink(v);
    }
    // ADD more
    return "iDom - unknown parameter: "+ v[1];
}

std::string command_iDom::help() const
{
    std::stringstream help;
    help << "iDom - for control smart home" << std::endl;
    help << "iDom speakers ON/OFF - to on or off speakers" << std::endl;
    help << "iDom 230v ON/OFF     - to on or off printers" << std::endl;
    help << "iDom 230V OUTDOOR - turn on off outdoor 230v socket" << std::endl;
    help << "iDom sunset/sunrise/day lenght - to show those parameters" << std::endl;
    help << "iDom sun        - get sunrise, sunset and day lenght" << std::endl;
    help << "iDom sysinfo    - get system info" << std::endl;
    help << "iDom text       - get text to speach" << std::endl;
    help << "iDom say <text> - say standatrd info or <text>" << std::endl;
    help << "iDom sms <text> - send sms<text>" << std::endl;
    help << "iDom temperature - get temperature from all termomether" << std::endl;
    help << "iDom temperature stats <name> - get temperature stats from termomether <name>" << std::endl;
    help << "iDom smog       - get current SMOG level (KRAKOW)" << std::endl;
    help << "iDom kill thread <ID>  - kill thread but denger!" << std::endl;
    help << "iDom camera LED ON/OFF - LED camera work" << std::endl;
    help << "iDom facebook ... - post on facebook wall" << std::endl;
    help << "iDom viber ...   - send viber msg" << std::endl;
    help << "iDom weather <city> <radius> - get weather alert" << std::endl;
    help << "iDom alarm ON/OFF hh:mm - set alarm clock" << std::endl;
    help << "iDom alarm SET from/to/radio <value> - set alarm clock" << std::endl;
    help << "iDom alarm GET - get alarm time" << std::endl;
    help << "iDom lock   - lock home" << std::endl;
    help << "iDom unlock - unlock home" << std::endl;
    help << "iDom KODI - start KODI smart TV" << std::endl;
    help << "iDom wifi - show all wifi client" << std::endl;
    help << "iDom health - get iDom server alarm health" << std::endl;
    help << "iDom key add ?tmp? <name> <size> - add new access key" << std::endl;
    help << "iDom key remove <key name> - remove access key" << std::endl;
    help << "iDom link < - generate temporaty link for action" << std::endl;
    help << "iDom doorbell - on doorbell" << std::endl;
    help << "iDom blink - blink outdoor siren for 5 sek" << std::endl;
    help << "iDom isDay - get information about day"  << std::endl;
    return help.str();
}
