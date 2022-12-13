#include <algorithm>
#include <fstream>
#include <string>
#include <typeinfo>

#include "../../libs/emoji/emoji.h"
#include "../functions/functions.h"
#include "../RADIO_433_eq/radio_433_eq.h"
#include "../thread_functions/iDom_thread.h"

iDomTOOLS::iDomTOOLS(thread_data *myData):
    m_key(myData->server_settings->_server.TS_KEY),
    m_key2(myData->server_settings->_server.TS_KEY2)
{
    my_data = myData;

    my_data->m_keyHandler = std::make_unique<iDomKEY_ACCESS>( iDomKEY_ACCESS(myData->server_settings->_server.keyDatabasePath));
    //////////////////////////////////// temeprature /////////////////
    m_allThermometer.add("inside");
    m_allThermometer.add("outside");
    m_allThermometerUpdate.add("inside");
    m_allThermometerUpdate.add("outside");
    /////////////////////////////////////////////////////////////////

    my_data->main_iDomStatus->addObject("cameraLED", STATE::UNKNOWN);
    my_data->main_iDomStatus->addObject("printer", STATE::OFF);
    my_data->main_iDomStatus->addObject("speakers", STATE::OFF);
    my_data->main_iDomStatus->addObject("alarm", STATE::DEACTIVE);
    my_data->main_iDomStatus->addObject("KODI", STATE::DEACTIVE);

    ///////// setup viber api
    m_viber.setAvatar(my_data->server_settings->_fb_viber.viberAvatar);
    m_viber.setAccessToken(my_data->server_settings->_fb_viber.viberToken);
    m_viber.setURL("https://chatapi.viber.com/pa/send_message");
    ///////// setup faceboook api
    m_facebook.setAccessToken(my_data->server_settings->_fb_viber.facebookAccessToken);

    //////// button 433MHz
    m_buttonPointerVector = my_data->main_REC->getButtonPointerVector();
    m_lastButton433MHzLockUnlockTime = Clock::getTime() + Clock(23,58);

    iDom_API::m_className.append(typeid(this).name());
    iDom_API::addToMap(m_className,this);
}

iDomTOOLS::~iDomTOOLS(){
    iDom_API::removeFromMap(m_className);
}

TEMPERATURE_STATE iDomTOOLS::getTHERMOMETER_CONTAINERlastState(const std::string& name)
{
    return m_allThermometer.getLastState(name);
}

TEMPERATURE_STATE iDomTOOLS::hasTemperatureChange(const std::string& thermometerName,
                                                  double reference, double histereza )
{
    reference += 0.0055;
    const auto newTemp = m_allThermometer.getTemp(thermometerName);
    const auto oldTemp = m_allThermometer.getOldTemp(thermometerName);
    const auto lastState = m_allThermometer.getLastState(thermometerName);
    if (newTemp >= reference + histereza &&
            oldTemp < reference + histereza &&
            lastState not_eq TEMPERATURE_STATE::Over)
    {
        my_data->myEventHandler.run("test")->addEvent("over: new " + to_string_with_precision(newTemp) + " old: "
                                                      + to_string_with_precision(oldTemp) + " ref: "
                                                      + to_string_with_precision(reference));
        m_allThermometer.setState(thermometerName, TEMPERATURE_STATE::Over);
        return TEMPERATURE_STATE::Over;
    }
    else if (newTemp <= reference - histereza &&
             oldTemp > reference - histereza &&
             lastState not_eq TEMPERATURE_STATE::Under)
    {
        my_data->myEventHandler.run("test")->addEvent("under: new " + to_string_with_precision(newTemp) + " old: "
                                                      + to_string_with_precision(oldTemp) + " ref: "
                                                      + to_string_with_precision(reference));
        m_allThermometer.setState(thermometerName, TEMPERATURE_STATE::Under);
        return TEMPERATURE_STATE::Under;
    }

    my_data->myEventHandler.run("test")->addEvent("noChanges: new " + to_string_with_precision(newTemp) + " old: "
                                                  + to_string_with_precision(oldTemp) + " ref: "
                                                  + to_string_with_precision(reference));
    m_allThermometer.setState(thermometerName, TEMPERATURE_STATE::NoChanges);
    return TEMPERATURE_STATE::NoChanges;
}

void iDomTOOLS::sendSMSifTempChanged(const std::string& thermomethernName, int reference)
{
    TEMPERATURE_STATE status = hasTemperatureChange(thermomethernName,reference,0.5);
    std::string m = "temperature " + thermomethernName + " over " + EMOJI::emoji(E_emoji::NORTH_EAST_ARROW)
            + to_string_with_precision(reference);

    if (status == TEMPERATURE_STATE::Over)
    {
        my_data->myEventHandler.run("temperature")->addEvent(m);
        if (reference < 2)
        {
            sendViberMsg(m,my_data->server_settings->_fb_viber.viberReceiver.at(0),
                         my_data->server_settings->_fb_viber.viberSender);
            sendViberMsg(m,my_data->server_settings->_fb_viber.viberReceiver.at(1),
                         my_data->server_settings->_fb_viber.viberSender);
        }
        else
        {
            sendViberMsg(m,my_data->server_settings->_fb_viber.viberReceiver.at(0),
                         my_data->server_settings->_fb_viber.viberSender);
        }
        my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      m);

    }
    else if (status == TEMPERATURE_STATE::Under)
    {
        m = "temperature " + thermomethernName + " under " + EMOJI::emoji(E_emoji::SOUTH_EAST_ARROW)
                + to_string_with_precision(reference);
        my_data->myEventHandler.run("temperature")->addEvent(m);
        if (reference < 2)
        {
            sendViberPicture(m, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTEU-fCklbx_ZFKaVhdGCymAg8NTldnva1GvnAEl63XfigJa2VV&s",
                             my_data->server_settings->_fb_viber.viberReceiver.at(0),
                             my_data->server_settings->_fb_viber.viberSender);
            sendViberPicture(m, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTEU-fCklbx_ZFKaVhdGCymAg8NTldnva1GvnAEl63XfigJa2VV&s",
                             my_data->server_settings->_fb_viber.viberReceiver.at(1),
                             my_data->server_settings->_fb_viber.viberSender);
            postOnFacebook(m, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTEU-fCklbx_ZFKaVhdGCymAg8NTldnva1GvnAEl63XfigJa2VV&s");
        }
        else {
            sendViberMsg(m,my_data->server_settings->_fb_viber.viberReceiver.at(0),
                         my_data->server_settings->_fb_viber.viberSender);
        }
        my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      m);
    }
}

std::string iDomTOOLS::getThermoStats(const std::string& name)
{
    return m_allThermometerUpdate.getStatsByName(name);
}

void iDomTOOLS::updateTemperatureStats()
{
    auto v = getTemperature();
    m_allThermometerUpdate.updateAll(&v);
    m_allThermometerUpdate.updateStats("outside");
    m_allThermometerUpdate.updateStats("inside");

    if( true == m_allThermometerUpdate.isMoreDiff("outside",2.1))
    {
        auto data = m_allThermometerUpdate.getLast2("outside");
        std::string msg = "alarm roznicy temeratur na polu! " + to_string_with_precision(data.first) + " na "
                + to_string_with_precision(data.second);

        if (data.first > data.second)
        {
            msg.append(" temperatura maleje " + EMOJI::emoji(E_emoji::CHART_WITH_DOWNWARDS_TREND));
        }
        else
        {
            msg.append(" temperatura rośnie " + EMOJI::emoji(E_emoji::CHART_WITH_UPWARDS_TREND));
        }

        sendViberMsg(msg,
                     my_data->server_settings->_fb_viber.viberReceiver.at(0),
                     my_data->server_settings->_fb_viber.viberSender);

        my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      msg);

        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << msg << std::endl;
        log_file_mutex.mutex_unlock();
    }

    if( true == m_allThermometerUpdate.isMoreDiff("inside",2.1))
    {
        auto data = m_allThermometerUpdate.getLast2("inside");
        std::string msg = "alarm roznicy temeratur na mieszkaniu! " + to_string_with_precision(data.first)
                + " na " + to_string_with_precision(data.second);

        if (data.first > data.second)
        {
            msg.append(" temperatura maleje " + EMOJI::emoji(E_emoji::CHART_WITH_DOWNWARDS_TREND));
        }
        else
        {
            msg.append(" temperatura rośnie " + EMOJI::emoji(E_emoji::CHART_WITH_UPWARDS_TREND));
        }
        sendViberMsg(msg,
                     my_data->server_settings->_fb_viber.viberReceiver.at(0),
                     my_data->server_settings->_fb_viber.viberSender);

        my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      msg);

        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << msg << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::turnOnSpeakers()
{
    if (my_data->idom_all_state.houseState == STATE::UNLOCK)
    {
        digitalWrite(iDomConst::GPIO_SPIK, HIGH);
        //FIXME temp fix
        my_data->main_iDomTools->turnOn433MHzSwitch("fan");
        ///////////////////////
        my_data->main_iDomStatus->setObjectState("speakers",STATE::ON);
    }
    else
    {
        my_data->myEventHandler.run("speakers")->addEvent("speakers can not start due to home state: " +
                                                          stateToString(my_data->idom_all_state.houseState));
    }
    my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
}

void iDomTOOLS::turnOffSpeakers()
{
    digitalWrite(iDomConst::GPIO_SPIK, LOW);
    //FIXME temp fix
    my_data->main_iDomTools->turnOff433MHzSwitch("fan");
    /////////////////////////////
    my_data->main_iDomStatus->setObjectState("speakers", STATE::OFF);
    my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
}

void iDomTOOLS::turnOnPrinter()
{
    if (my_data->idom_all_state.houseState == STATE::UNLOCK)
    {
        digitalWrite(iDomConst::GPIO_PRINTER,HIGH);
        my_data->myEventHandler.run("230V")->addEvent("230v drukarki ON");
        my_data->main_iDomStatus->setObjectState("printer",STATE::ON);
    }
    else
    {
        my_data->myEventHandler.run("230V")->addEvent("Printer can not start due to home state: "
                                                      + stateToString(my_data->idom_all_state.houseState));
    }
}

void iDomTOOLS::turnOffPrinter()
{
    digitalWrite(iDomConst::GPIO_PRINTER,LOW);
    my_data->myEventHandler.run("230V")->addEvent("230v drukarki OFF");
    my_data->main_iDomStatus->setObjectState("printer",STATE::OFF);
}

PIN_STATE iDomTOOLS::getPinState(int pin_number)
{
    int pin_state = digitalRead(pin_number);
    switch (pin_state){
    case 0:
        return PIN_STATE::LOW_STATE;
    case 1:
        return PIN_STATE::HIGH_STATE;
    default:
        return PIN_STATE::UNKNOWN_STATE;
    }
}

void iDomTOOLS::turnOnOff230vOutdoor()
{
    unsigned int ID = 99;
    auto state = my_data->main_house_room_handler->m_lightingBulbMap.at(ID)->getStatus();
    if (state == STATE::ON)
        my_data->main_house_room_handler->turnOffBulb(ID);
    else
        my_data->main_house_room_handler->turnOnBulb(ID);
}

void iDomTOOLS::turnOnOff433MHzSwitch(const std::string& name)
{
    STATE listwaState = my_data->main_iDomStatus->getObjectState(name);
    RADIO_SWITCH *m_switch = nullptr;
    try
    {
        m_switch = dynamic_cast<RADIO_SWITCH*>(my_data->main_REC->getEqPointer(name));
    }
    catch (const std::string& e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "void iDomTOOLS::turnOnOff433MHzSwitch(const std::string& name)"
                      << e << std::endl;
        log_file_mutex.mutex_unlock();
        return;
    }

    if (listwaState == STATE::ON)
    {
        // my_data->mainLCD->set_lcd_STATE(10);
        // my_data->mainLCD->printString(true,0,0,"230V OFF " + name);
        m_switch->off();
    }
    else if (listwaState == STATE::OFF)
    {
        // my_data->mainLCD->set_lcd_STATE(10);
        //my_data->mainLCD->printString(true,0,0,"230V ON " + name);
        m_switch->on();
    }
    saveState_iDom(my_data->serverStarted);
}

void iDomTOOLS::turnOn433MHzSwitch(std::string name)
{
    try
    {
        auto v_switch = my_data->main_REC->getSwitchPointerVector();
        for(auto s : v_switch)
        {
            if(useful_F_libs::hasSubstring(s->getName(),name) == true)
            {
                s->on();
            }
        }
    }
    catch (const std::string& e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "void iDomTOOLS::turnOn433MHzSwitch(std::string name)"
                      << e << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::turnOff433MHzSwitch(std::string name)
{
    try
    {
        auto v_switch = my_data->main_REC->getSwitchPointerVector();
        for(auto s : v_switch)
        {
            if(useful_F_libs::hasSubstring(s->getName(),name) == true)
            {
                s->off();
            }
        }
    }
    catch (const std::string& e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "void iDomTOOLS::turnOff433MHzSwitch(std::string name)"
                      << e << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::runOnSunset()
{

    if(my_data->server_settings->_command.contains("sunset")){
        if(my_data->idom_all_state.houseState == STATE::LOCK
                and my_data->server_settings->_command["sunset"].contains("lock"))
        {
            runCommandFromJson(my_data->server_settings->_command["sunset"]["lock"].get<std::vector<std::string>>());
        }
        else if(my_data->idom_all_state.houseState == STATE::UNLOCK
                and my_data->server_settings->_command["sunset"].contains("unlock"))
        {
            runCommandFromJson(my_data->server_settings->_command["sunset"]["unlock"].get<std::vector<std::string>>());
        }
    }

    my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/sun","SUNSET");
    my_data->m_keyHandler->removeExpiredKeys(8);
}

void iDomTOOLS::runOnSunrise()
{
    if(my_data->server_settings->_command.contains("sunrise")){
        if(my_data->idom_all_state.houseState == STATE::LOCK
                and my_data->server_settings->_command["sunrise"].contains("lock"))
        {
            runCommandFromJson(my_data->server_settings->_command["sunrise"]["lock"].get<std::vector<std::string>>());
        }
        else if(my_data->idom_all_state.houseState == STATE::UNLOCK
                and my_data->server_settings->_command["sunrise"].contains("unlock"))
        {
            runCommandFromJson(my_data->server_settings->_command["sunrise"]["unlock"].get<std::vector<std::string>>());
        }
    }
    my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/sun", "SUNRISE");
}

void iDomTOOLS::lockHome()
{
    if(my_data->idom_all_state.houseState == STATE::LOCK){
        return;
    }

    // arm alarm
    if(my_data->idom_all_state.alarmSatelState != STATE::ARMED  and
            my_data->server_settings->_runThread.SATEL == true)
        my_data->satelIntegraHandler->getSatelPTR()->armAlarm(my_data->server_settings->_satel_integra.partitionID);

    // run command
    if(my_data->server_settings->_command.contains("lock")){
        runCommandFromJson(my_data->server_settings->_command["lock"].get<std::vector<std::string>>());
    }

    my_data->main_iDomTools->sendViberPicture("dom zablokownay!",
                                              "http://45.90.3.84/images/iDom/iDom/lock.jpg",
                                              my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                              my_data->server_settings->_fb_viber.viberSender);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "zablokowanie domu - "  <<(my_data->idom_all_state.houseState) << std::endl;
    log_file_mutex.mutex_unlock();

    saveState_iDom(my_data->serverStarted);

    if( my_data->server_settings->_runThread.SATEL == true){
        my_data->satelIntegraHandler->getSatelPTR()->outputOn(my_data->server_settings->_satel_integra.outdoor_siren_lights_id); //turn on satel output to blink outdoor siren
    }

    my_data->idom_all_state.houseState = STATE::LOCK;
    my_data->main_iDomStatus->setObjectState("house", STATE::LOCK);
}

void iDomTOOLS::unlockHome()
{
    if(my_data->idom_all_state.houseState == STATE::UNLOCK)
        return;

    my_data->idom_all_state.houseState = STATE::UNLOCK;
    my_data->idom_all_state.counter = 0;
    my_data->main_iDomStatus->setObjectState("house", STATE::UNLOCK);

    // disarm alarm
    if(my_data->idom_all_state.alarmSatelState != STATE::DISARMED and
            my_data->server_settings->_runThread.SATEL == true)
        my_data->satelIntegraHandler->getSatelPTR()->disarmAlarm(my_data->server_settings->_satel_integra.partitionID);

    // run command
    if(my_data->server_settings->_command.contains("unlock")){
       runCommandFromJson(my_data->server_settings->_command["unlock"].get<std::vector<std::string>>());
    }

    // send message on viber
    my_data->main_iDomTools->sendViberPicture("dom odblokownay!",
                                              "http://45.90.3.84/images/iDom/iDom/unlock.jpg",
                                              my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                              my_data->server_settings->_fb_viber.viberSender);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "odblokowanie domu - " << my_data->idom_all_state.houseState << std::endl;
    log_file_mutex.mutex_unlock();

    saveState_iDom(my_data->serverStarted);

    if(my_data->server_settings->_runThread.SATEL == true){
        my_data->satelIntegraHandler->getSatelPTR()->outputOn(my_data->server_settings->_satel_integra.outdoor_siren_lights_id); //turn on satel output to blink outdoor siren
    }
}

void iDomTOOLS::buttonLockHome()
{
    ledOFF();
    MPD_stop();
    turnOffPrinter();
    lockHome();
}

void iDomTOOLS::buttonUnlockHome()
{
    unlockHome();
    MPD_play(my_data);
}

bool iDomTOOLS::isItDay()
{
    Clock now = Clock::getTime();
    if(now < iDomTOOLS::getSunriseClock() or now > iDomTOOLS::getSunsetClock()){
        return false;
    }
    return true;
}

std::string iDomTOOLS::getAllDataSunriseSunset()
{
    return m_sun.getAllData();
}

void iDomTOOLS::checkLightning()
{
    return ;
   // log_file_mutex.mutex_lock();
   // log_file_cout << ERROR << "nie wysłano informacje o burzy" << std::endl;
   // log_file_mutex.mutex_unlock();
}


std::string iDomTOOLS::getSunrise(bool extend )
{
    Clock tt = m_sun.getSunRise();
    if (extend == true){
        return "Sunrise time: " + tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getSunset(bool extend )
{
    Clock tt = m_sun.getSunSet();
    if (extend == true){
        return "Sunset time: " + tt.getString();
    }
    return tt.getString();
}

Clock iDomTOOLS::getSunsetClock()
{
    return m_sun.getSunSet();
}

Clock iDomTOOLS::getSunriseClock()
{
    return m_sun.getSunRise();
}

std::string iDomTOOLS::getDayLenght(bool extend )
{
    Clock tt = m_sun.getDayLength();
    if (extend == true){
        return "Day Lenght : " + tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getWeatherEvent(const std::string& city, unsigned int radius)
{
    std::string url = "http://burze.dzis.net/ramka.php?miejscowosc=";
    url.append(city);
    url.append("&promien=");
    url.append(std::to_string(radius));
    return useful_F_libs::httpPost(url, 10);
}

WEATHER_DATABASE iDomTOOLS::getAlert()
{
    std::string bufferData = useful_F_libs::httpPost(my_data->server_settings->_server.lightningApiURL);
    std::string d = useful_F_libs::removeHtmlTag(bufferData);

    auto vect = useful_F::split(d,'\n');
    vect.pop_back();

    WEATHER_DATABASE wAlert;

    wAlert.lightning.alert = vect.at(0);

    return wAlert;
}

void iDomTOOLS::textToSpeach(std::vector<std::string> *textVector)
{
    if (textVector->empty() ){
        return;
    }
    std::string txt;

    for (const auto& a : *textVector){
        txt.append(a);
    }
    /////////// start thread TTS - python use ////////////////////////
    std::string command = " python /home/pi/programowanie/iDom_server_OOP/script/PYTHON/gadacz.py \\" + txt + "\\";
    if(my_data->ptr_MPD_info->isPlay == false){
        turnOnSpeakers();
    }

    useful_F::runLinuxCommand(command);

    if(my_data->ptr_MPD_info->isPlay == false){
        turnOffSpeakers();
    }
}

std::string iDomTOOLS::getTextToSpeach()
{
    std::vector<std::string> dayL = useful_F::split(getDayLenght(),':');
    std::stringstream text;
    std::string smogText = getSmog();
    int smogInt = std::stoi(smogText);
    text << "Godzina: " << Clock::getTime().getString();
    text << ". \nWschód słońca: " << getSunrise();
    text << ". \nZachód słońca: " << getSunset();
    text << ". \nDługość dnia: " << dayL[0] << " godzin " << dayL[1] << " minut";
    text <<". \n";
    dayL = getTemperature();
    text << "Temperatura na zewnątrz: " << dayL[1] << " stopnia. \n";
    text << "Temperatura w pokoju: " << dayL[0] << " stopnia. \n";
    text << "Smog: " << smogText << " mg/m^3. \n";
    if (smogInt > 50){
        int result = smogInt *2;
        text << "UWAGA! Maksymalna wartość przekroczona " << result << "%.";
    }
    return text.str();
}

std::vector<std::string> iDomTOOLS::getTemperature()
{
    std::vector<std::string> vect;
    vect.push_back(to_string_with_precision(my_data->ptr_buderus->getInsideTemp()));
    vect.push_back(to_string_with_precision(my_data->ptr_buderus->getOutdoorTemp()));
    std::string msg("Inside: ");
    msg.append(vect[0]);
    msg.append(" Outside: ");
    msg.append(vect[1]);
    my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/temperature",msg);
    return vect;
}

std::string iDomTOOLS::getTemperatureString()
{
//    std::stringstream str;
//    str << std::setprecision(4)
//        << my_data->ptr_buderus->getInsideTemp() << ":"
//        << my_data->ptr_buderus->getOutdoorTemp() << ":"
//        << my_data->ptr_buderus->getBoilerTemp() << ":"
//        << getFloorTemp();
//    return str.str();
    nlohmann::json jj;
    jj["inside"] = my_data->ptr_buderus->getInsideTemp();
    jj["outdoor"] = my_data->ptr_buderus->getOutdoorTemp();
    jj["boiler"] = my_data->ptr_buderus->getBoilerTemp();
    jj["floor"] = std::stod(getFloorTemp());
    jj["currentFlow"] = my_data->ptr_buderus->getCurFlowTemp();

    return jj.dump();
}

std::string iDomTOOLS::getSmog()
{
    std::string  addres = "https://api.gios.gov.pl/pjp-api/rest/data/getData/20320";
    std::string readBuffer = useful_F_libs::httpPost(addres, 10);
    try {
        auto jj = nlohmann::json::parse(readBuffer);
        int i = 1;
        while(true){
            readBuffer = jj["values"][i]["value"].dump();
            if(readBuffer != "null" or i == 10)
                break;
            ++i;
        }

    }
    catch (...){
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek substr() e getSmog() !!!!!!" << std::endl;
        log_file_mutex.mutex_unlock();
        return  "-1";
    }

    my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/smog", readBuffer);
    return readBuffer;
}

void iDomTOOLS::send_data_to_thingSpeak()
{
    //get temperature in gardener house
    RADIO_WEATHER_STATION* st = static_cast<RADIO_WEATHER_STATION*>(my_data->main_REC->getEqPointer("first"));
    auto temp = st->data.getTemperature();


    std::vector<std::string> _temperature = getTemperature();
    std::stringstream addres;
    addres << "api.thingspeak.com/update?key=";
    addres << m_key;
    addres << "&field1=" << _temperature.at(1);
    addres << "&field2=" << _temperature.at(0);
    addres << "&field3=" << to_string_with_precision(my_data->lusina.statTemp.average());
    addres << "&field4=" << my_data->lusina.statHumi.average();
    addres << "&field5=" << getSmog();
    addres << "&field6=" << to_string_with_precision(my_data->ptr_buderus->getBoilerTemp());
    addres << "&field7=" << my_data->ptr_buderus->isHeatingActiv();
    addres << "&field8=" << temp;
    //////////////////////////////// pozyskanie temperatury
    m_allThermometer.updateAll(&_temperature);
    sendSMSifTempChanged("outside",0);
    sendSMSifTempChanged("inside",24);
    std::string s = useful_F_libs::httpPost(addres.str(), 10);

    std::string s2 = "0";
    try{
        addres.str("");
        addres << "api.thingspeak.com/update?key=";
        addres << m_key2;
        addres << "&field1=" << my_data->lusina.shedBat.average();
        addres << "&field2=" << my_data->lusina.shedPres.average();
        addres << "&field3=" << my_data->lusina.shedHum.average();
        addres << "&field4=" << my_data->lusina.shedTemp.average();
        addres << "&field5=" << my_data->ptr_buderus->getCurFlowTemp();
        s2 = useful_F_libs::httpPost(addres.str(), 10);
    }
    catch(...){
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << " błąd wysyłania temoeratury na thingspeak brak jsona" << std::endl;
        log_file_mutex.mutex_unlock();
        s2 = "1";
    }

    if(s == "0" or s2 == "0"){
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << " błąd wysyłania temoeratury na thingspeak s: " << s   << " s2: " << s2<< std::endl;
        log_file_mutex.mutex_unlock();
    }
}

std::string iDomTOOLS::getFloorTemp()
{
    return my_data->lusina.temperatureDS20;
}

void iDomTOOLS::cameraLedON(const std::string& link)
{
    Clock t = Clock::getTime();
    SunRiseSet sun;
    Clock sunRise, sunSet;
    sunRise = sun.getSunRise();
    sunSet = sun.getSunSet();
    sunSet += Clock(23,30); // +23:30 == -00:30
    if (t <= sunRise or t >= sunSet){
        std::string s = useful_F_libs::httpPost(link,10);
        if (s == "ok.\n"){
            my_data->main_iDomStatus->setObjectState("cameraLED", STATE::ON);
        }
    }
}

void iDomTOOLS::cameraLedOFF(const std::string& link)
{
    std::string s = useful_F_libs::httpPost(link, 10);
    if (s == "ok.\n"){
        my_data->main_iDomStatus->setObjectState("cameraLED", STATE::OFF);
    }
}

nlohmann::json iDomTOOLS::sendViberMsg(const std::string &msg,
                                       const std::string &receiver,
                                       const std::string &senderName,
                                       const std::string& accessToken,
                                       const std::string& url)
{
    nlohmann::json jj;
    std::lock_guard<std::mutex> lock(m_msgMutex);
    try {
        jj = nlohmann::json::parse( m_viber.sendViberMSG(msg,receiver,senderName,accessToken,url));
    }  catch (...) {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek json w wysylaniu sendViberMsg !!!!!!" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    return jj;
}

nlohmann::json iDomTOOLS::sendViberPicture(const std::string &msg,
                                           const std::string &image,
                                           const std::string &receiver,
                                           const std::string &senderName,
                                           const std::string& accessToken,
                                           const std::string& url)
{
    nlohmann::json jj;
    std::lock_guard<std::mutex> lock(m_msgMutex);
    try {
        jj = nlohmann::json::parse(m_viber.sendViberPicture(msg, image, receiver, senderName, accessToken, url));
    }  catch (...) {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek json w wysylaniu sendViberPicture() !!!!!!" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    return jj;
}

STATE iDomTOOLS::sendViberMsgBool(const std::string &msg,
                                  const std::string &receiver,
                                  const std::string &senderName,
                                  const std::string& accessToken,
                                  const std::string& url)
{
    nlohmann::json jj = sendViberMsg(msg,receiver,senderName,accessToken,url);
    STATE ret = STATE::SEND_NOK;
    if(jj.find("status_message") not_eq jj.end())
    {
        if(jj.at("status_message").get<std::string>() == "ok")
        {
            ret = STATE::SEND_OK;
        }
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "nie wyslanno wiadomosci viber" << jj.dump() << std::endl;
        log_file_mutex.mutex_unlock();
    }
    return ret;
}

STATE iDomTOOLS::sendViberPictureBool(const std::string& msg,
                                      const std::string& image,
                                      const std::string& receiver,
                                      const std::string& senderName,
                                      const std::string& accessToken,
                                      const std::string& url)
{
    nlohmann::json jj = sendViberPicture(msg, image, receiver, senderName, accessToken, url);
    STATE ret = STATE::SEND_NOK;
    if(jj.at("status_message").get<std::string>() == "ok")
    {
        ret = STATE::SEND_OK;
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "nie wyslanno wiadomosci viber" << jj.dump() << std::endl;
        log_file_mutex.mutex_unlock();
    }
    return ret;
}

std::string iDomTOOLS::postOnFacebook(const std::string& msg, const std::string& image)
{
    std::lock_guard<std::mutex> lock(m_msgMutex);
    if (image not_eq "NULL"){
        return m_facebook.postPhotoOnWall(image,msg);
    }

    return m_facebook.postTxtOnWall(msg);
}

std::string iDomTOOLS::ledOFF()
{
    my_data->main_iDomStatus->setObjectState("Night_Light", STATE::OFF);
    // temporary
    turnOff433MHzSwitch("B");
    return "done";
}

std::string iDomTOOLS::ledClear()
{
    return "done";
}

std::string iDomTOOLS::ledClear(unsigned int from, unsigned int to)
{
    return "Led cleared";
}

std::string iDomTOOLS::ledOn()
{
    turnOn433MHzSwitch("B");
    return "done";
}

void iDomTOOLS::checkAlarm()
{
    unsigned int fromVol = my_data->alarmTime.fromVolume;
    unsigned int toVol  = my_data->alarmTime.toVolume;
    unsigned int radioId = my_data->alarmTime.radioID;

    Clock now = Clock::getTime();
    if (now == my_data->alarmTime.time && my_data->alarmTime.state == STATE::ACTIVE){
        my_data->alarmTime.state = STATE::WORKING;
        MPD_volumeSet(my_data, static_cast<int>(fromVol));
        MPD_play(my_data, static_cast<int>(radioId));
        my_data->main_iDomStatus->setObjectState("alarm",STATE::DEACTIVE);
        my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                      stateToString(STATE::WORKING));

        auto topic = my_data->server_settings->_mqtt_broker.topicSubscribe;
        topic.pop_back();
        for(const auto & command : my_data->alarmTime.commands)
        {
            my_data->mqttHandler->publish(topic +"command", command );
        }
    }

    if (my_data->alarmTime.state == STATE::WORKING){
        auto vol = static_cast<unsigned int>(MPD_getVolume(my_data) + 1);
        if (vol < toVol){
            MPD_volumeSet(my_data, static_cast<int>(vol));
        }
        else{
            my_data->alarmTime.state = STATE::DEACTIVE;
            my_data->main_Rs232->print("LED_CLOCK:0;");
            my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                          stateToString(STATE::DEACTIVE));
            if(iDomTOOLS::isItDay() == false){
                my_data->main_iDomTools->turnOn433MHzSwitch("ALARM");
                saveState_iDom(my_data->serverStarted);
                log_file_mutex.mutex_lock();
                log_file_cout << DEBUG << "uruchamiam ALARM 433MHz"<< std::endl;
                log_file_mutex.mutex_unlock();
            }
        }
    }
}

void iDomTOOLS::saveState_iDom(const bool& started)
{
    if (started == false)
    {
        return;
    }
    iDom_SAVE_STATE info(my_data->server_settings->_server.saveFilePath);
    nlohmann::json jsonAlarm;
    nlohmann::json jsonMPD;
    nlohmann::json json_iDomLOCK;
    nlohmann::json json433Mhz;
    //////////////////// iDom
    json_iDomLOCK["iDomLock"] = stateToString(my_data->idom_all_state.houseState);
    //////////////////// alarm
    jsonAlarm["alarm"] = my_data->main_iDomStatus->getObjectStateString("alarm");
    jsonAlarm["time"] = my_data->alarmTime.time.getString();
    jsonAlarm["fromVolume"] = my_data->alarmTime.fromVolume;
    jsonAlarm["toVolume"] = my_data->alarmTime.toVolume;
    jsonAlarm["radioID"] = my_data->alarmTime.radioID;
    jsonAlarm["commands"] = my_data->alarmTime.commands;
    //////////////////// mpd
    jsonMPD["music"] = my_data->main_iDomStatus->getObjectStateString("music");
    jsonMPD["speakers"] = my_data->main_iDomStatus->getObjectStateString("speakers");
    ////////////////// 433Mhz
    auto switch433vector = my_data->main_REC->getSwitchPointerVector();
    for (auto v : switch433vector)
    {
        v->getName();
        json433Mhz[v->getName()] = stateToString(v->getState());
    }
    ///
    nlohmann::json json;
    json["iDom"] = json_iDomLOCK;
    json["ALARM"] = jsonAlarm;
    json["MPD"] = jsonMPD;
    json["433Mhz"] = json433Mhz;

    info.write(json);

    my_data->mqttHandler->publishRetained(my_data->server_settings->_mqtt_broker.topicPublish + "/state", json.dump(4));

#ifdef BT_TEST
    std::cout << json <<std::endl;
    std::cout << " saved to " << my_data->server_settings->_server.saveFilePath << std::endl;
#endif
}

void iDomTOOLS::readState_iDom(nlohmann::json jj)
{
    try
    {
#ifdef BT_TEST
        std::cout << "JSON: " << jj.dump(4) << std::endl;
#endif
        nlohmann::json json433MHz = jj.at("433Mhz");

        for (nlohmann::json::iterator it = json433MHz.begin(); it not_eq json433MHz.end(); ++it)
        {
            if( it.value() == "ON"){
                my_data->main_iDomTools->turnOn433MHzSwitch(it.key());
            }
            else if ( it.value() == "OFF"){
                my_data->main_iDomTools->turnOff433MHzSwitch(it.key());
            }
        }
        auto iDomLock = jj.at("iDom").at("iDomLock").get<std::string>();

        if(iDomLock == "UNLOCK")
            unlockHome();
        else if (iDomLock == "LOCK")
            lockHome();

        auto mpdMusic = jj.at("MPD").at("music").get<std::string>();
        auto mpdSpeakers = jj.at("MPD").at("speakers").get<std::string>();

        if(mpdMusic == "PLAY")
            MPD_play(my_data);
        else if(mpdMusic == "STOP")
            MPD_stop();
        if(mpdSpeakers == "ON")
            turnOnSpeakers();
        else if(mpdSpeakers == "OFF")
            turnOffSpeakers();

        auto alarmState = jj.at("ALARM").at("alarm").get<std::string>();
        auto alarmTime = jj.at("ALARM").at("time").get<std::string>();
        my_data->alarmTime.fromVolume = jj.at("ALARM").at("fromVolume").get<unsigned int>();
        my_data->alarmTime.toVolume = jj.at("ALARM").at("toVolume").get<unsigned int>();
        my_data->alarmTime.radioID = jj.at("ALARM").at("radioID").get<unsigned int>();
        my_data->alarmTime.time = Clock(alarmTime);
        for(const auto& k : jj.at("ALARM").at("commands"))
        {
            my_data->alarmTime.commands.push_back(k);
        }

        if (alarmState == "ACTIVE"){
            my_data->alarmTime.state = STATE::ACTIVE;
            my_data->main_iDomStatus->setObjectState("alarm", my_data->alarmTime.state);
            saveState_iDom(my_data->serverStarted);
        }
    }
    catch(...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "nie ma pliku json z stanem iDom" << std::endl;
        log_file_mutex.mutex_unlock();
#ifdef BT_TEST
        std::cout << "nie ma pliku json z stanem iDom" << std::endl;
#endif
    }
}

std::string iDomTOOLS::startKodi_Thread()
{
    STATE kodiState = my_data->main_iDomStatus->getObjectState("KODI");
    if (kodiState == STATE::ACTIVE)
        return "kodi already run";
    return iDOM_THREAD::start_thread("kodi smartTV", useful_F::kodi, my_data);
}

void iDomTOOLS::wifiClientConnected()
{
    buttonUnlockHome();
}

void iDomTOOLS::wifiClientDisconnected()
{
    buttonLockHome();
}

void iDomTOOLS::doorbellDingDong()
{
    try {
        RADIO_SWITCH *m_switch = dynamic_cast<RADIO_SWITCH*>(my_data->main_REC->getEqPointer("DingDong"));
        m_switch->onFor15sec();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Dzwonek do drzwi" << std::endl;
        log_file_mutex.mutex_unlock();
    }  catch (...) {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "brak dzwonka do drzwi!!! w paśmie 433MHz" << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::runCommandFromJson(const std::vector<std::string> &jj)
{
    CommandHandlerMQTT commandMQTT;
    for(const auto& command : jj){
        auto v = useful_F::split(command, ' ');
#ifdef BT_TEST
        std::cout << "command to run: " << command << std::endl;
#endif
        commandMQTT.run(v, my_data);
    }
}
