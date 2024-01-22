#include <optional>

#include "../../libs/emoji/emoji.h"
#include "../functions/functions.h"
#include "../RADIO_433_eq/radio_433_eq.h"
#include "../thread_functions/iDom_thread.h"
#include "idomtools.h"
#include "../iDomSaveState/idom_save_state.h"
#include "../dbClient/db-client-factory.hpp"

iDomTOOLS::iDomTOOLS(thread_data *myData) : m_key(myData->server_settings->_server.TS_KEY),
                                            m_key2(myData->server_settings->_server.TS_KEY2)
{
    context = myData;

    context->m_keyHandler = std::make_unique<iDomKEY_ACCESS>(iDomKEY_ACCESS(myData->server_settings->_server.keyDatabasePath));
    //////////////////////////////////// temeprature /////////////////
    m_allThermometer.add("inside");
    m_allThermometer.add("outside");
    m_allThermometerUpdate.add("inside");
    m_allThermometerUpdate.add("outside");
    /////////////////////////////////////////////////////////////////

    context->main_iDomStatus->addObject("printer", STATE::OFF);
    context->main_iDomStatus->addObject("speakers", STATE::OFF);
    context->main_iDomStatus->addObject("alarm", STATE::DEACTIVE);
    context->main_iDomStatus->addObject("KODI", STATE::DEACTIVE);

    m_viber_notif = context->server_settings->_server.viber_notification;

    ///////// setup viber api
    m_viber.setAvatar(context->server_settings->_fb_viber.viberAvatar);
    m_viber.setAccessToken(context->server_settings->_fb_viber.viberToken);
    m_viber.setURL("https://chatapi.viber.com/pa/send_message");

    //////// button 433MHz
    m_buttonPointerVector = context->main_REC->getButtonPointerVector();
    m_lastButton433MHzLockUnlockTime = Clock::getTime() + Clock(23, 58);

    iDom_API::m_className.insert(0, typeid(this).name());
    iDom_API::addToMap(m_className, this);
}

iDomTOOLS::~iDomTOOLS()
{
    iDom_API::removeFromMap(m_className);
}

TEMPERATURE_STATE iDomTOOLS::getTHERMOMETER_CONTAINERlastState(const std::string &name)
{
    return m_allThermometer.getLastState(name);
}

TEMPERATURE_STATE iDomTOOLS::hasTemperatureChange(const std::string &thermometerName,
                                                  double reference, double histereza)
{
    reference += 0.0055;
    const auto newTemp = m_allThermometer.getTemp(thermometerName);
    const auto oldTemp = m_allThermometer.getOldTemp(thermometerName);
    const auto lastState = m_allThermometer.getLastState(thermometerName);
    if (newTemp >= reference + histereza &&
        oldTemp < reference + histereza &&
        lastState not_eq TEMPERATURE_STATE::Over)
    {
        context->myEventHandler.run("test")->addEvent("over: new " + to_string_with_precision(newTemp) + " old: " + to_string_with_precision(oldTemp) + " ref: " + to_string_with_precision(reference));
        m_allThermometer.setState(thermometerName, TEMPERATURE_STATE::Over);
        return TEMPERATURE_STATE::Over;
    }
    else if (newTemp <= reference - histereza &&
             oldTemp > reference - histereza &&
             lastState not_eq TEMPERATURE_STATE::Under)
    {
        context->myEventHandler.run("test")->addEvent("under: new " + to_string_with_precision(newTemp) + " old: " + to_string_with_precision(oldTemp) + " ref: " + to_string_with_precision(reference));
        m_allThermometer.setState(thermometerName, TEMPERATURE_STATE::Under);
        return TEMPERATURE_STATE::Under;
    }

    context->myEventHandler.run("test")->addEvent("noChanges: new " + to_string_with_precision(newTemp) + " old: " + to_string_with_precision(oldTemp) + " ref: " + to_string_with_precision(reference));
    m_allThermometer.setState(thermometerName, TEMPERATURE_STATE::NoChanges);
    return TEMPERATURE_STATE::NoChanges;
}

void iDomTOOLS::sendSMSifTempChanged(const std::string &thermomethernName, int reference)
{
    TEMPERATURE_STATE status = hasTemperatureChange(thermomethernName, reference, 0.5);
    std::string m = "temperature " + thermomethernName + " over " + EMOJI::emoji(E_emoji::NORTH_EAST_ARROW) + to_string_with_precision(reference);

    if (status == TEMPERATURE_STATE::Over)
    {
        context->myEventHandler.run("temperature")->addEvent(m);
        if (reference < 2)
        {
            sendViberMsg(m, context->server_settings->_fb_viber.viberReceiver.at(0),
                         context->server_settings->_fb_viber.viberSender);
            sendViberMsg(m, context->server_settings->_fb_viber.viberReceiver.at(1),
                         context->server_settings->_fb_viber.viberSender);
        }
        else
        {
            sendViberMsg(m, context->server_settings->_fb_viber.viberReceiver.at(0),
                         context->server_settings->_fb_viber.viberSender);
        }
        context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      m);
    }
    else if (status == TEMPERATURE_STATE::Under)
    {
        m = "temperature " + thermomethernName + " under " + EMOJI::emoji(E_emoji::SOUTH_EAST_ARROW) + to_string_with_precision(reference);
        context->myEventHandler.run("temperature")->addEvent(m);
        if (reference < 2)
        {
            sendViberPicture(m, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTEU-fCklbx_ZFKaVhdGCymAg8NTldnva1GvnAEl63XfigJa2VV&s",
                             context->server_settings->_fb_viber.viberReceiver.at(0),
                             context->server_settings->_fb_viber.viberSender);
            sendViberPicture(m, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTEU-fCklbx_ZFKaVhdGCymAg8NTldnva1GvnAEl63XfigJa2VV&s",
                             context->server_settings->_fb_viber.viberReceiver.at(1),
                             context->server_settings->_fb_viber.viberSender);
        }
        else
        {
            sendViberMsg(m, context->server_settings->_fb_viber.viberReceiver.at(0),
                         context->server_settings->_fb_viber.viberSender);
        }
        context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      m);
    }
}

std::string iDomTOOLS::getThermoStats(const std::string &name)
{
    return m_allThermometerUpdate.getStatsByName(name);
}

void iDomTOOLS::updateTemperatureStats()
{
    auto v = getTemperature();
    m_allThermometerUpdate.updateAll(&v);
    m_allThermometerUpdate.updateStats("outside");
    m_allThermometerUpdate.updateStats("inside");

    if (true == m_allThermometerUpdate.isMoreDiff("outside", 2.1))
    {
        auto data = m_allThermometerUpdate.getLast2("outside");
        std::string msg = "alarm roznicy temeratur na polu! " + to_string_with_precision(data.first) + " na " + to_string_with_precision(data.second);

        if (data.first > data.second)
        {
            msg.append(" temperatura maleje " + EMOJI::emoji(E_emoji::CHART_WITH_DOWNWARDS_TREND));
        }
        else
        {
            msg.append(" temperatura rośnie " + EMOJI::emoji(E_emoji::CHART_WITH_UPWARDS_TREND));
        }

        sendViberMsg(msg,
                     context->server_settings->_fb_viber.viberReceiver.at(0),
                     context->server_settings->_fb_viber.viberSender);

        context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      msg);

        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << msg << std::endl;
        log_file_mutex.mutex_unlock();
    }

    if (true == m_allThermometerUpdate.isMoreDiff("inside", 2.1))
    {
        auto data = m_allThermometerUpdate.getLast2("inside");
        std::string msg = "alarm roznicy temeratur na mieszkaniu! " + to_string_with_precision(data.first) + " na " + to_string_with_precision(data.second);

        if (data.first > data.second)
        {
            msg.append(" temperatura maleje " + EMOJI::emoji(E_emoji::CHART_WITH_DOWNWARDS_TREND));
        }
        else
        {
            msg.append(" temperatura rośnie " + EMOJI::emoji(E_emoji::CHART_WITH_UPWARDS_TREND));
        }
        sendViberMsg(msg,
                     context->server_settings->_fb_viber.viberReceiver.at(0),
                     context->server_settings->_fb_viber.viberSender);

        context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/temperature",
                                      msg);

        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << msg << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::turnOnSpeakers()
{
    if (context->idom_all_state.houseState == STATE::UNLOCK)
    {
        // FIXME temp fix
        context->main_iDomTools->turnOn433MHzSwitch("fan");
        ///////////////////////
        context->main_iDomStatus->setObjectState("speakers", STATE::ON);
    }
    else
    {
        context->myEventHandler.run("speakers")->addEvent("speakers can not start due to home state: " + stateToString(context->idom_all_state.houseState));
    }
    context->main_iDomTools->saveState_iDom(context->serverStarted);
}

void iDomTOOLS::turnOffSpeakers()
{
    // digitalWrite(iDomConst::GPIO_SPIK, LOW);
    //  FIXME temp fix
    context->main_iDomTools->turnOff433MHzSwitch("fan");
    /////////////////////////////
    context->main_iDomStatus->setObjectState("speakers", STATE::OFF);
    context->main_iDomTools->saveState_iDom(context->serverStarted);
}

void iDomTOOLS::turnOnOff230vOutdoor()
{
    unsigned int ID = 99;
    auto state = context->main_house_room_handler->m_lightingBulbMap.at(ID)->getStatus();
    if (state == STATE::ON)
        context->main_house_room_handler->turnOffBulb(ID);
    else
        context->main_house_room_handler->turnOnBulb(ID);
}

void iDomTOOLS::turnOnOff433MHzSwitch(const std::string &name)
{
    STATE listwaState = context->main_iDomStatus->getObjectState(name);
    RADIO_SWITCH *m_switch = nullptr;
    try
    {
        m_switch = dynamic_cast<RADIO_SWITCH *>(context->main_REC->getEqPointer(name));
    }
    catch (const std::string &e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "void iDomTOOLS::turnOnOff433MHzSwitch(const std::string& name)"
                      << e << std::endl;
        log_file_mutex.mutex_unlock();
        return;
    }

    if (listwaState == STATE::ON)
    {
        // context->mainLCD->set_lcd_STATE(10);
        // context->mainLCD->printString(true,0,0,"230V OFF " + name);
        m_switch->off();
    }
    else if (listwaState == STATE::OFF)
    {
        // context->mainLCD->set_lcd_STATE(10);
        // context->mainLCD->printString(true,0,0,"230V ON " + name);
        m_switch->on();
    }
    saveState_iDom(context->serverStarted);
}

void iDomTOOLS::turnOn433MHzSwitch(std::string name)
{
    try
    {
        auto v_switch = context->main_REC->getSwitchPointerVector();
        for (auto s : v_switch)
        {
            if (useful_F_libs::hasSubstring(s->getName(), name) == true)
            {
                s->on();
            }
        }
    }
    catch (const std::string &e)
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
        auto v_switch = context->main_REC->getSwitchPointerVector();
        for (auto s : v_switch)
        {
            if (useful_F_libs::hasSubstring(s->getName(), name) == true)
            {
                s->off();
            }
        }
    }
    catch (const std::string &e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "void iDomTOOLS::turnOff433MHzSwitch(std::string name)"
                      << e << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::runOnSunset()
{

    if (context->server_settings->_command.contains("sunset"))
    {
        if (context->idom_all_state.houseState == STATE::LOCK and context->server_settings->_command["sunset"].contains("lock"))
        {
            runCommandFromJson(context->server_settings->_command["sunset"]["lock"].get<std::vector<std::string>>());
        }
        else if (context->idom_all_state.houseState == STATE::UNLOCK and context->server_settings->_command["sunset"].contains("unlock"))
        {
            runCommandFromJson(context->server_settings->_command["sunset"]["unlock"].get<std::vector<std::string>>());
        }
    }

    context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/sun", "SUNSET");
    context->m_keyHandler->removeExpiredKeys(8);
}

void iDomTOOLS::runOnSunrise()
{
    if (context->server_settings->_command.contains("sunrise"))
    {
        if (context->idom_all_state.houseState == STATE::LOCK and context->server_settings->_command["sunrise"].contains("lock"))
        {
            runCommandFromJson(context->server_settings->_command["sunrise"]["lock"].get<std::vector<std::string>>());
        }
        else if (context->idom_all_state.houseState == STATE::UNLOCK and context->server_settings->_command["sunrise"].contains("unlock"))
        {
            runCommandFromJson(context->server_settings->_command["sunrise"]["unlock"].get<std::vector<std::string>>());
        }
    }
    context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/sun", "SUNRISE");
}

void iDomTOOLS::lockHome()
{
    if (context->idom_all_state.houseState == STATE::LOCK)
    {
        return;
    }

    // arm alarm
    if (context->idom_all_state.alarmSatelState != STATE::ARMED and
        context->server_settings->_runThread.SATEL == true)
        context->satelIntegraHandler->getSatelPTR()->armAlarm(context->server_settings->_satel_integra.partitionID);

    // run command
    if (context->server_settings->_command.contains("lock"))
    {
        runCommandFromJson(context->server_settings->_command["lock"].get<std::vector<std::string>>());
    }

    context->main_iDomTools->sendViberPicture("dom zablokownay!",
                                              "https://cyniu88.no-ip.pl/images/iDom/iDom/lock.jpg",
                                              context->server_settings->_fb_viber.viberReceiver.at(0),
                                              context->server_settings->_fb_viber.viberSender);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "zablokowanie domu - " << (context->idom_all_state.houseState) << std::endl;
    log_file_mutex.mutex_unlock();

    saveState_iDom(context->serverStarted);

    if (context->server_settings->_runThread.SATEL == true)
    {
        context->satelIntegraHandler->getSatelPTR()->outputOn(context->server_settings->_satel_integra.outdoor_siren_lights_id); // turn on satel output to blink outdoor siren
    }

    context->idom_all_state.houseState = STATE::LOCK;
    context->main_iDomStatus->setObjectState("house", STATE::LOCK);
}

void iDomTOOLS::unlockHome()
{
    if (context->idom_all_state.houseState == STATE::UNLOCK)
        return;

    context->idom_all_state.houseState = STATE::UNLOCK;
    context->idom_all_state.counter = 0;
    context->main_iDomStatus->setObjectState("house", STATE::UNLOCK);

    // disarm alarm
    if (context->idom_all_state.alarmSatelState != STATE::DISARMED and
        context->server_settings->_runThread.SATEL == true)
        context->satelIntegraHandler->getSatelPTR()->disarmAlarm(context->server_settings->_satel_integra.partitionID);

    // run command
    if (context->server_settings->_command.contains("unlock"))
    {
        runCommandFromJson(context->server_settings->_command["unlock"].get<std::vector<std::string>>());
    }

    // send message on viber
    context->main_iDomTools->sendViberPicture("dom odblokownay!",
                                              "https://cyniu88.no-ip.pl/images/iDom/iDom/unlock.jpg",
                                              context->server_settings->_fb_viber.viberReceiver.at(0),
                                              context->server_settings->_fb_viber.viberSender);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "odblokowanie domu - " << context->idom_all_state.houseState << std::endl;
    log_file_mutex.mutex_unlock();

    saveState_iDom(context->serverStarted);

    if (context->server_settings->_runThread.SATEL == true)
    {
        context->satelIntegraHandler->getSatelPTR()->outputOn(context->server_settings->_satel_integra.outdoor_siren_lights_id); // turn on satel output to blink outdoor siren
    }
}

void iDomTOOLS::buttonLockHome()
{
    ledOFF();
    MPD_stop();
    lockHome();
}

void iDomTOOLS::buttonUnlockHome()
{
    unlockHome();
    MPD_play(context);
}

bool iDomTOOLS::isItDay()
{
    Clock now = Clock::getTime();
    if (now < iDomTOOLS::getSunriseClock() or now > iDomTOOLS::getSunsetClock())
    {
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
    return;
    // log_file_mutex.mutex_lock();
    // log_file_cout << ERROR << "nie wysłano informacje o burzy" << std::endl;
    // log_file_mutex.mutex_unlock();
}

std::string iDomTOOLS::getSunrise(bool extend)
{
    Clock tt = m_sun.getSunRise();
    if (extend == true)
    {
        return "Sunrise time: " + tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getSunset(bool extend)
{
    Clock tt = m_sun.getSunSet();
    if (extend == true)
    {
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

std::string iDomTOOLS::getDayLenght(bool extend)
{
    Clock tt = m_sun.getDayLength();
    if (extend == true)
    {
        return "Day Lenght : " + tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getWeatherEvent(const std::string &city, unsigned int radius)
{
    std::string url = "http://burze.dzis.net/ramka.php?miejscowosc=";
    url.append(city);
    url.append("&promien=");
    url.append(std::to_string(radius));
    return useful_F_libs::httpPost(url, 10);
}

WEATHER_DATABASE iDomTOOLS::getAlert()
{
    std::string bufferData = useful_F_libs::httpPost(context->server_settings->_server.lightningApiURL);
    std::string d = useful_F_libs::removeHtmlTag(bufferData);

    auto vect = useful_F::split(d, '\n');
    vect.pop_back();

    WEATHER_DATABASE wAlert;

    wAlert.lightning.alert = vect.at(0);

    return wAlert;
}

void iDomTOOLS::textToSpeach(std::vector<std::string> *textVector)
{
    if (textVector->empty())
    {
        return;
    }
    std::string txt;

    for (const auto &a : *textVector)
    {
        txt.append(a);
    }
    /////////// start thread TTS - python use ////////////////////////
    std::string command = " python /home/pi/programowanie/iDom_server_OOP/script/PYTHON/gadacz.py \\" + txt + "\\";
    if (context->ptr_MPD_info->isPlay == false)
    {
        turnOnSpeakers();
    }

    useful_F::runLinuxCommand(command);

    if (context->ptr_MPD_info->isPlay == false)
    {
        turnOffSpeakers();
    }
}

std::string iDomTOOLS::getTextToSpeach()
{
    std::vector<std::string> dayL = useful_F::split(getDayLenght(), ':');
    std::stringstream text;
    std::string smogText = getSmog().value_or("0");
    int smogInt = std::stoi(smogText);
    text << "Godzina: " << Clock::getTime().getString();
    text << ". \nWschód słońca: " << getSunrise();
    text << ". \nZachód słońca: " << getSunset();
    text << ". \nDługość dnia: " << dayL[0] << " godzin " << dayL[1] << " minut";
    text << ". \n";
    dayL = getTemperature();
    text << "Temperatura na zewnątrz: " << dayL[1] << " stopnia. \n";
    text << "Temperatura w pokoju: " << dayL[0] << " stopnia. \n";
    text << "Smog: " << smogText << " mg/m^3. \n";
    if (smogInt > 50)
    {
        int result = smogInt * 2;
        text << "UWAGA! Maksymalna wartość przekroczona " << result << "%.";
    }
    return text.str();
}

std::vector<std::string> iDomTOOLS::getTemperature()
{
    std::vector<std::string> vect;
    vect.push_back(to_string_with_precision(context->ptr_buderus->getInsideTemp()));
    vect.push_back(to_string_with_precision(context->ptr_buderus->getOutdoorTemp()));
    std::string msg("Inside: ");
    msg.append(vect[0]);
    msg.append(" Outside: ");
    msg.append(vect[1]);
    context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/temperature", msg);
    return vect;
}

std::string iDomTOOLS::getTemperatureString()
{
    //    std::stringstream str;
    //    str << std::setprecision(4)
    //        << context->ptr_buderus->getInsideTemp() << ":"
    //        << context->ptr_buderus->getOutdoorTemp() << ":"
    //        << context->ptr_buderus->getBoilerTemp() << ":"
    //        << getFloorTemp();
    //    return str.str();
    nlohmann::json jj;
    jj["inside"] = context->ptr_buderus->getInsideTemp();
    jj["outdoor"] = context->ptr_buderus->getOutdoorTemp();
    jj["boiler"] = context->ptr_buderus->getBoilerTemp();
    jj["floor"] = std::stod(getFloorTemp());
    jj["currentFlow"] = context->ptr_buderus->getCurFlowTemp();

    return jj.dump();
}

std::optional<std::string> iDomTOOLS::getSmog()
{
    std::string addres = "https://api.gios.gov.pl/pjp-api/rest/data/getData/20320";
    std::string readBuffer = useful_F_libs::httpPost(addres, 20);
    std::optional<std::string> ret;
    try
    {
        auto jj = nlohmann::json::parse(readBuffer);
        int i = 1;
        while (true)
        {
            readBuffer = jj["values"][i]["value"].dump();
            if (readBuffer != "null" or i == 4)
            {
                break;
            }
            ++i;
        }
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek substr() e getSmog() !!!!!!" << std::endl;
        log_file_cout << CRITICAL << "getSmog() return: " << readBuffer << std::endl;
        log_file_mutex.mutex_unlock();
        return std::nullopt;
    }
    if (readBuffer != "null")
        ret = readBuffer;
    context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/smog", readBuffer);
    return ret;
}

void iDomTOOLS::send_data_to_thingSpeak()
{
    // get temperature in gardener house
    RADIO_WEATHER_STATION *st = static_cast<RADIO_WEATHER_STATION *>(context->main_REC->getEqPointer("first"));
    auto temp = st->data.getTemperature();

    std::vector<std::string> _temperature = getTemperature();
    std::stringstream addres;
    addres << "api.thingspeak.com/update?key=";
    addres << m_key;
    addres << "&field1=" << _temperature.at(1);
    addres << "&field2=" << _temperature.at(0);
    auto smog = getSmog();
    if (smog.has_value())
        addres << "&field5=" << getSmog().value();
    addres << "&field6=" << to_string_with_precision(context->ptr_buderus->getBoilerTemp());
    addres << "&field7=" << context->ptr_buderus->isHeatingActiv();
    if (temp.has_value())
        addres << "&field8=" << temp.value();
    //////////////////////////////// pozyskanie temperatury
    m_allThermometer.updateAll(&_temperature);
    sendSMSifTempChanged("outside", 0);
    sendSMSifTempChanged("inside", 24);
    std::string s = useful_F_libs::httpPost(addres.str(), 10);

    std::string s2 = "0";
    try
    {
        addres.str("");
        addres << "api.thingspeak.com/update?key=";
        addres << m_key2;
        addres << "&field1=" << context->lusina.shedFloor.average();
        addres << "&field2=" << context->lusina.shedPres.average();
        addres << "&field3=" << context->lusina.shedHum.average();
        addres << "&field4=" << context->lusina.shedTemp.average();
        addres << "&field5=" << context->ptr_buderus->getCurFlowTemp();
        s2 = useful_F_libs::httpPost(addres.str(), 10);
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << " błąd wysyłania temperatury na thingspeak brak jsona" << std::endl;
        log_file_mutex.mutex_unlock();
        s2 = "1";
    }

    if (s == "0" or s2 == "0")
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << " błąd wysyłania temperatury na thingspeak s: " << s << " s2: " << s2 << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::send_data_to_influxdb()
{
    try
    {
        RADIO_WEATHER_STATION *st = static_cast<RADIO_WEATHER_STATION *>(context->main_REC->getEqPointer("first"));
        std::optional<double> temp = st->data.getTemperature();

        std::vector<std::string> _temperature = getTemperature();

        std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> iDomData;

        iDomData["temperatura"]["outdoor"] = std::stof(_temperature.at(1));
        iDomData["temperatura"]["inside"] = std::stof(_temperature.at(0));
        iDomData["temperatura"]["floor"] = context->lusina.shedFloor.average();
        iDomData["temperatura"]["bojler"] = context->ptr_buderus->getBoilerTemp();
        iDomData["temperatura"]["domek"] = temp.value();
        iDomData["temperatura"]["flow"] = context->ptr_buderus->getCurFlowTemp();
        iDomData["temperatura"]["shedTemp"] = context->lusina.shedTemp.average();

        iDomData["wilgoc"]["humi"] = context->lusina.shedHum.average();
        auto smog = getSmog();
        if (smog.has_value())
            iDomData["smog"]["smog"] = std::stof(smog.value());
        else
            iDomData["smog"]["smog"] = std::nullopt;
        iDomData["cisnienie"]["dom"] = context->lusina.shedPres.average();
        iDomData["piec"]["praca"] = context->ptr_buderus->isHeatingActiv();
        iDomData["acdc"]["acdc"] = context->lusina.acdc.average();

        dbClientFactory dbFactory;
        auto db = dbFactory.createDbClient();
        auto returnCode = db->upload_iDomData(iDomData);

        if (returnCode != 204)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << CRITICAL << " błąd wysyłania danych iDom do influxdb " << returnCode << " " << reasonPhrase(returnCode) << std::endl;
            log_file_mutex.mutex_unlock();
            throw 55;
        }
    }
    catch (std::exception &e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << " błąd (wyjatek) wysyłania temperatury do influxdb " << e.what() << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

std::string iDomTOOLS::getFloorTemp()
{
    return std::to_string(context->lusina.shedFloor.average());
}

nlohmann::json iDomTOOLS::sendViberMsg(const std::string &msg,
                                       const std::string &receiver,
                                       const std::string &senderName,
                                       const std::string &accessToken,
                                       const std::string &url)
{
    nlohmann::json jj;
    if (m_viber_notif == false)
        return jj;
    std::lock_guard<std::mutex> lock(m_msgMutex);
    try
    {
        jj = nlohmann::json::parse(m_viber.sendViberMSG(msg, receiver, senderName, accessToken, url));
    }
    catch (...)
    {
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
                                           const std::string &accessToken,
                                           const std::string &url)
{
    nlohmann::json jj;
    if (m_viber_notif == false)
        return jj;
    std::lock_guard<std::mutex> lock(m_msgMutex);
    try
    {
        jj = nlohmann::json::parse(m_viber.sendViberPicture(msg, image, receiver, senderName, accessToken, url));
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek json w wysylaniu sendViberPicture() !!!!!!" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    return jj;
}

nlohmann::json iDomTOOLS::sendViberUrl(const std::string &msg, const std::string &url2, const std::string &receiver, const std::string &senderName, const std::string &accessToken, const std::string &url)
{
    nlohmann::json jj;
    if (m_viber_notif == false)
        return jj;
    std::lock_guard<std::mutex> lock(m_msgMutex);
    try
    {
        jj = nlohmann::json::parse(m_viber.sendViberUrl(msg, url2, receiver, senderName, accessToken, url));
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek json w wysylaniu sendViberPicture() !!!!!!" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    return jj;
}

STATE iDomTOOLS::sendViberMsgBool(const std::string &msg,
                                  const std::string &receiver,
                                  const std::string &senderName,
                                  const std::string &accessToken,
                                  const std::string &url)
{
    nlohmann::json jj;
    if (m_viber_notif == false)
        return jj;
    jj = sendViberMsg(msg, receiver, senderName, accessToken, url);
    STATE ret = STATE::SEND_NOK;
    if (jj.find("status_message") not_eq jj.end())
    {
        if (jj.at("status_message").get<std::string>() == "ok")
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

STATE iDomTOOLS::sendViberPictureBool(const std::string &msg,
                                      const std::string &image,
                                      const std::string &receiver,
                                      const std::string &senderName,
                                      const std::string &accessToken,
                                      const std::string &url)
{
    nlohmann::json jj = sendViberPicture(msg, image, receiver, senderName, accessToken, url);
    STATE ret = STATE::SEND_NOK;
    if (m_viber_notif == false)
        return ret;
    if (jj.at("status_message").get<std::string>() == "ok")
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

std::string iDomTOOLS::ledOFF()
{
    context->main_iDomStatus->setObjectState("Night_Light", STATE::OFF);
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
    unsigned int fromVol = context->alarmTime.fromVolume;
    unsigned int toVol = context->alarmTime.toVolume;
    unsigned int radioId = context->alarmTime.radioID;

    Clock now = Clock::getTime();
    if (now == context->alarmTime.time && context->alarmTime.state == STATE::ACTIVE)
    {
        context->alarmTime.state = STATE::WORKING;
        MPD_volumeSet(context, static_cast<int>(fromVol));
        MPD_play(context, static_cast<int>(radioId));
        context->main_iDomStatus->setObjectState("alarm", STATE::DEACTIVE);
        context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                      stateToString(STATE::WORKING));

        auto topic = context->server_settings->_mqtt_broker.topicSubscribe;
        topic.pop_back();
        for (const auto &command : context->alarmTime.commands)
        {
            context->mqttHandler->publish(topic + "command", command);
        }
    }

    if (context->alarmTime.state == STATE::WORKING)
    {
        auto vol = static_cast<unsigned int>(MPD_getVolume(context) + 1);
        if (vol < toVol)
        {
            MPD_volumeSet(context, static_cast<int>(vol));
        }
        else
        {
            context->alarmTime.state = STATE::DEACTIVE;
            context->main_Rs232->print("LED_CLOCK:0;");
            context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/alarm",
                                          stateToString(STATE::DEACTIVE));
            if (iDomTOOLS::isItDay() == false)
            {
                context->main_iDomTools->turnOn433MHzSwitch("ALARM");
                saveState_iDom(context->serverStarted);
                log_file_mutex.mutex_lock();
                log_file_cout << DEBUG << "uruchamiam ALARM 433MHz" << std::endl;
                log_file_mutex.mutex_unlock();
            }
        }
    }
}

void iDomTOOLS::saveState_iDom(const bool &started)
{
    if (started == false)
    {
        return;
    }
    iDom_SAVE_STATE info(context->server_settings->_server.saveFilePath);
    nlohmann::json jsonAlarm;
    nlohmann::json jsonMPD;
    nlohmann::json json_iDomLOCK;
    nlohmann::json json433Mhz;
    //////////////////// iDom
    json_iDomLOCK["iDomLock"] = stateToString(context->idom_all_state.houseState);
    //////////////////// alarm
    jsonAlarm["alarm"] = context->main_iDomStatus->getObjectStateString("alarm");
    jsonAlarm["time"] = context->alarmTime.time.getString();
    jsonAlarm["fromVolume"] = context->alarmTime.fromVolume;
    jsonAlarm["toVolume"] = context->alarmTime.toVolume;
    jsonAlarm["radioID"] = context->alarmTime.radioID;
    jsonAlarm["commands"] = context->alarmTime.commands;
    //////////////////// mpd
    jsonMPD["music"] = context->main_iDomStatus->getObjectStateString("music");
    jsonMPD["speakers"] = context->main_iDomStatus->getObjectStateString("speakers");
    ////////////////// 433Mhz
    auto switch433vector = context->main_REC->getSwitchPointerVector();
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

    context->mqttHandler->publishRetained(context->server_settings->_mqtt_broker.topicPublish + "/state", json.dump(4));

#ifdef BT_TEST
    std::cout << json << std::endl;
    std::cout << " saved to " << context->server_settings->_server.saveFilePath << std::endl;
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
            if (it.value() == "ON")
            {
                context->main_iDomTools->turnOn433MHzSwitch(it.key());
            }
            else if (it.value() == "OFF")
            {
                context->main_iDomTools->turnOff433MHzSwitch(it.key());
            }
        }
        auto iDomLock = jj.at("iDom").at("iDomLock").get<std::string>();

        if (iDomLock == "UNLOCK")
            unlockHome();
        else if (iDomLock == "LOCK")
            lockHome();

        auto mpdMusic = jj.at("MPD").at("music").get<std::string>();
        auto mpdSpeakers = jj.at("MPD").at("speakers").get<std::string>();

        if (mpdMusic == "PLAY")
            MPD_play(context);
        else if (mpdMusic == "STOP")
            MPD_stop();
        if (mpdSpeakers == "ON")
            turnOnSpeakers();
        else if (mpdSpeakers == "OFF")
            turnOffSpeakers();

        auto alarmState = jj.at("ALARM").at("alarm").get<std::string>();
        auto alarmTime = jj.at("ALARM").at("time").get<std::string>();
        context->alarmTime.fromVolume = jj.at("ALARM").at("fromVolume").get<unsigned int>();
        context->alarmTime.toVolume = jj.at("ALARM").at("toVolume").get<unsigned int>();
        context->alarmTime.radioID = jj.at("ALARM").at("radioID").get<unsigned int>();
        context->alarmTime.time = Clock(alarmTime);
        for (const auto &k : jj.at("ALARM").at("commands"))
        {
            context->alarmTime.commands.push_back(k);
        }

        if (alarmState == "ACTIVE")
        {
            context->alarmTime.state = STATE::ACTIVE;
            context->main_iDomStatus->setObjectState("alarm", context->alarmTime.state);
            saveState_iDom(context->serverStarted);
        }
    }
    catch (...)
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
    STATE kodiState = context->main_iDomStatus->getObjectState("KODI");
    if (kodiState == STATE::ACTIVE)
        return "kodi already run";
    return iDOM_THREAD::start_thread("kodi smartTV", useful_F::kodi, context);
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
    try
    {
        RADIO_SWITCH *m_switch = dynamic_cast<RADIO_SWITCH *>(context->main_REC->getEqPointer("DingDong"));
        m_switch->onFor15sec();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Dzwonek do drzwi" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "brak dzwonka do drzwi!!! w paśmie 433MHz" << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::runCommandFromJson(const std::vector<std::string> &jj)
{
    CommandHandlerMQTT commandMQTT;
    for (const auto &command : jj)
    {
        auto v = useful_F::split(command, ' ');
#ifdef BT_TEST
        std::cout << "command to run: " << command << std::endl;
#endif
        commandMQTT.run(v, context);
    }
}
