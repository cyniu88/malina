#include "command_idom.h"
#include "../../iDomTools/idomtools_interface.h"

command_iDom::command_iDom(const std::string &name):command(name)
{
}

std::string command_iDom::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size()<2){
        return "need parameter!\n" + help();
    }
    if (v[1] == "speakers")
    {
        if (v[2] == "ON"){
            my_data->main_iDomTools->turnOnSpeakers();
            return "speakers ON";
        }
        else if (v[2] == "OFF"){
            my_data->main_iDomTools->turnOffSpeakers();
            return "speakers OFF";
        }
        else{
            return "unknow speakers action: "+v[2];
        }
    }
    else if (v[1] == "sunset"){
        return my_data->main_iDomTools->getSunset(true);
    }
    else if (v[1] == "sunrise"){
        return my_data->main_iDomTools->getSunrise(true);
    }
    else if (v[1] == "day" && v[2] == "lenght"){
        return my_data->main_iDomTools->getDayLenght(true);
    }
    else if (v[1] == "sun"){
        std::string ret;
        ret = my_data->main_iDomTools->getSunrise(true);
        ret.append("\n");
        ret.append(my_data->main_iDomTools->getSunset(true));
        ret.append("\n");
        ret.append(my_data->main_iDomTools->getDayLenght(true));
        ret.append("\n");
        return ret;
    }
    else if (v[1] == "sysinfo"){
        return my_data->main_iDomTools->getSystemInfo();
    }
    else if (v[1] == "lusina") //TODO temorary
    {
        std::stringstream ret;
        ret << my_data->lusina.statHumi.stats() <<std::endl;
        ret << my_data->lusina.statTemp.stats();
        return ret.str();
    }
    else if (v[1] == "temperature"){
        if(v.size() < 3){
            return my_data->main_iDomTools->getTemperatureString();
        }
        else {
            if (v[2] == "stats"){
                std::string ret;
                try{
                    ret = my_data->main_iDomTools->getThermoStats(v[3]);
                }
                catch (std::string& obj){
                    ret = obj + " " + v[3];
                }
                return ret;
            }
        }
    }
    else if (v[1] == "text"){
        return my_data->main_iDomTools->getTextToSpeach();
    }
    else if (v[1] == "lock"){
        my_data->main_iDomTools->lockHome();
        return "house locked";
    }
    else if (v[1] == "unlock"){
        my_data->main_iDomTools->unlockHome();
        return "house unlocked";
    }
    else if (v[1] == "say"){
        if (v.size() > 3){
            std::vector<std::string> vTTS = { my_data->main_iDomTools->getTextToSpeach()};
            my_data->main_iDomTools->textToSpeach(&vTTS);
            return "sad";
        }
    }
    else if (v[1] == "smog"){
        return my_data->main_iDomTools->getSmog() + " mg/m^3";
    }
    else if (v[1] == "230V"){
        if (v.size() > 2 && v[2] == "ON"){
            //my_data->main_iDomTools->turnOnPrinter();
            return "230V ON";
        }
        else if(v.size() > 2 && v[2] == "OFF"){
            //my_data->main_iDomTools->turnOffPrinter();
            return "230V OFF";
        }
        else if(v.size() > 2 && v[2] == "OUTDOOR"){
            my_data->main_iDomTools->turnOnOff230vOutdoor();
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
            my_data->main_THREAD_arr->at(std::stoul(v[3]) ).thread.~thread();
            return "done!";
        }
    }
    else if (v[1] == "facebook"){
        std::string msg;
        for (unsigned int i = 2; i < v.size(); ++i){
            msg.push_back(' ');
            msg.append(v[i]);
        }
        return my_data->main_iDomTools->postOnFacebook(msg);
    }
    else if (v[1] == "viber"){
        std::string msg;
        for (unsigned int i = 2; i < v.size(); ++i){
            msg.push_back(' ');
            msg.append(v[i]);
        }
        STATE stMSG = my_data->main_iDomTools->sendViberMsgBool(msg, my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                                my_data->server_settings->_fb_viber.viberSender);
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
            my_data->main_iDomTools->cameraLedON(my_data->server_settings->_camera.cameraLedON);
        }
        else if (v[2] == "LED" && v[3] == "OFF"){
            my_data->main_iDomTools->cameraLedOFF(my_data->server_settings->_camera.cameraLedOFF);
        }
        return "led DONE";
    }
    else if (v[1] == "weather"){

        if (v.size() < 4){
            return "not enough parameters";
        }
        std::string tempHTML = my_data->main_iDomTools->getWeatherEvent(v[2],std::stoi(v[3]));
        return useful_F_libs::removeHtmlTag(tempHTML);
    }
    else if (v[1] == "KODI"){
        return my_data->main_iDomTools->startKodi_Thread();
    }
    else if (v[1] == "health") {
        return my_data->iDomAlarm.showAlarm();
    }
    else if (v[1] == "isDay"){
        return my_data->main_iDomTools->isItDay() ? "isDay:true" : "isDay:false";
    }
    else if (v[1] == "blink"  and
             my_data->server_settings->_runThread.SATEL == true) {
        my_data->satelIntegraHandler->getSatelPTR()->outputOn(my_data->server_settings->_satel_integra.outdoor_siren_lights_id); //turn on satel output to blink outdoor siren
        return "done " + std::to_string(counter);
    }
    else if (v[1] == "doorbell") {
        // my_data->main_iDomTools->sendViberPicture("DZWONEK do bramy!",
        //                                           "https://png.pngtree.com/element_our/20190529/ourmid/pngtree-ring-the-doorbell-icon-image_1198163.jpg",
        //                                           my_data->server_settings->_fb_viber.viberReceiver.at(0),
        //                                           my_data->server_settings->_fb_viber.viberSender);   // inform  door bell has been pressed
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Dzwonek do bramy"<< std::endl;
        log_file_mutex.mutex_unlock();
        //TODO add doorbell actions if needed
        my_data->main_house_room_handler->turnOnAllInRoom("dzwonek");
        if(my_data->satelIntegraHandler != std::nullptr_t()){
            my_data->satelIntegraHandler->getSatelPTR()->outputOn(my_data->server_settings->_satel_integra.outdoor_siren_lights_id); //turn on satel output to blink outdoor siren in case of gate doorbell
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
            my_data->alarmTime.state = STATE::DEACTIVE;
            my_data->main_Rs232->print("LED_CLOCK:0;");
            my_data->main_iDomStatus->setObjectState("alarm", my_data->alarmTime.state);
            my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
            my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                          stateToString(STATE::DEACTIVE));
            return "alarm clock has been deactivated";
        }
        else if (v[2] == "GET"){
            return my_data->alarmTime.time.getString();
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
            else if (v[3] == "command")
            {
                my_data->alarmTime.commands.push_back("light room pokoj_N on");
                my_data->alarmTime.commands.push_back("light room korytarz on");
            }
            std::stringstream ret;
            ret << "The values has beedn set:" << std::endl;
            ret << "From Value: " << my_data->alarmTime.fromVolume << std::endl;
            ret << "To Value: " << my_data->alarmTime.toVolume << std::endl;
            ret << "Radio ID: " << my_data->alarmTime.radioID << std::endl;
            my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
            return ret.str();
        }
        else if (v[2] == "ON" && v.size() > 3){
            my_data->alarmTime.time = Clock(v[3]);
            my_data->alarmTime.state = STATE::ACTIVE;
            my_data->main_Rs232->print("LED_CLOCK:1;");
            my_data->main_iDomStatus->setObjectState("alarm", my_data->alarmTime.state);
            my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
            my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                          stateToString(STATE::ACTIVE));
            return "alarm clock has been activated";
        }

    }

    else if (v[1] == "key" && v[2] == "add")
    {
        if(v[3] == "tmp")
            my_data->m_keyHandler->addKEY(v[4],"null", std::stoul(v[5]),true);
        else {
            my_data->m_keyHandler->addKEY(v[3],"null", std::stoul(v[4]),false);
        }
        return my_data->m_keyHandler->listKEY();
    }
    else if (v[1] == "key" && v[2] == "remove" && v.size() == 4)
    {
        my_data->m_keyHandler->removeKEY(v[3]);
        return v[3] + " removed";
    }
    else if(v[1] == "link" && v.size() > 2)
    {
        v.erase(v.begin(), v.begin() + 2);
        return my_data->main_iDomTools->getLink(v);
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
