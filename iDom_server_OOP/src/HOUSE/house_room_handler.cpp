#include <fstream>

#include "house_room_handler.h"
#include "json.hpp"
#include "../functions/functions.h"
#include "../iDomTools/idomtools_interface.h"

std::string house_room_handler::m_mqttPublishTopic = "swiatlo/output/";

house_room_handler::house_room_handler(thread_context *context)
{
    m_bulbStatus.SetCapacity(100);
    m_className.insert(0, typeid(this).name());
    this->context = context;
    iDom_API::addToMap(m_className, this);
}

house_room_handler::~house_room_handler()
{
    iDom_API::removeFromMap(m_className);
}

void house_room_handler::loadConfig(const std::string &configPath)
{
    std::ifstream i(configPath);
    nlohmann::json j;
    i >> j;
    for (const auto &element : j)
    {
        std::string roomName = element.at("name").get<std::string>();
        std::vector<int> satelSensorID = element.at("satelSensorID").get<std::vector<int>>();
        std::map<int, std::shared_ptr<light_bulb>> lightingBulbMap;

        for (const auto &jj : element.at("bulb"))
        {
            std::string bulbName = jj.at("bulbName").get<std::string>();
            int bulbID = jj.at("bulbID").get<int>();
            lightingBulbMap.emplace(bulbID, std::make_shared<light_bulb>(roomName, bulbName, bulbID));
            lightingBulbMap[bulbID]->m_satelAlarm = stringToState(jj.at("satelAlarm").get<std::string>());
            lightingBulbMap[bulbID]->m_satelAlarmHowLong = jj.at("howLong").get<int>();

            for (const auto &kk : jj.at("switchID"))
            {
                lightingBulbMap.at(bulbID)->addBulbPin(kk.get<int>());
            }
            if (not m_lightingBulbMap.contains(bulbID))
            {
                m_lightingBulbMap[bulbID] = lightingBulbMap[bulbID];
            }
            else
            {
                log_file_mutex.mutex_lock();
                log_file_cout << ERROR << "sprawdz konfig zarowek! " << bulbID << " już istnieje" << std::endl;
                log_file_mutex.mutex_unlock();
            }
        }
        m_roomMap.emplace(roomName, std::make_shared<ROOM>(satelSensorID, roomName, lightingBulbMap));
        for (auto number : satelSensorID)
        {
            m_satelIdMap[number] = m_roomMap[roomName];
        }
    }
}

void house_room_handler::loadButtonConfig(const std::string &configPath)
{
    nlohmann::json jj;
    std::ifstream i(configPath);
    i >> jj;

    for (const auto &element : jj)
    {
        auto buttonID = element.at("buttonID").get<int>();
        if (element.contains("long"))
        {
            for (const auto &kk : element.at("long"))
            {
                m_buttonConfig[buttonID]["long"].push_back(kk.get<std::string>());
            }
        }
        if (element.contains("double"))
        {
            for (const auto &kk : element.at("double"))
            {
                m_buttonConfig[buttonID]["double"].push_back(kk.get<std::string>());
            }
        }
    }
}

void house_room_handler::turnOnAllInRoom(const std::string &roomName)
{
    if (not m_roomMap.contains(roomName))
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "sprawdz konfig zarowek! Pokój \"" << roomName << "\" nie istnieje" << std::endl;
        log_file_mutex.mutex_unlock();
        return;
    }
    for (auto &[key, value] : m_roomMap[roomName]->m_lightingBulbMap)
    {
        value->on([](const std::string &name)
                     { useful_F::myStaticCtx->mqttHandler->publish(m_mqttPublishTopic, name); });
    }
}

void house_room_handler::turnOffAllInRoom(const std::string &roomName)
{
    if (not m_roomMap.contains(roomName))
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "sprawdz konfig zarowek! Pokój \"" << roomName << "\" nie istnieje" << std::endl;
        log_file_mutex.mutex_unlock();
        return;
    }
    for (auto &[key, value] : m_roomMap[roomName]->m_lightingBulbMap)
    {
        value->off([](const std::string &name)
                      { useful_F::myStaticCtx->mqttHandler->publish(m_mqttPublishTopic, name); });
    }
}

void house_room_handler::changeAllInRoom(const std::string &roomName)
{
    if (not m_roomMap.contains(roomName))
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "sprawdz konfig zarowek! Pokój \"" << roomName << "\" nie istnieje" << std::endl;
        log_file_mutex.mutex_unlock();
        return;
    }
    for (auto &[key, value] : m_roomMap[roomName]->m_lightingBulbMap)
    {
        value->change([](const std::string &name)
                         { useful_F::myStaticCtx->mqttHandler->publish(m_mqttPublishTopic, name); });
    }
}

void house_room_handler::turnOnAllBulb()
{
    for (auto &[key, value] : m_roomMap)
    {
        turnOnAllInRoom(value->m_name);
    }
}

void house_room_handler::turnOffAllBulb()
{
    for (auto &[key, value] : m_roomMap)
    {
        turnOffAllInRoom(value->m_name);
    }
}

void house_room_handler::turnOnBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->on([](const std::string &name)
                                     { useful_F::myStaticCtx->mqttHandler->publish(m_mqttPublishTopic, name); });
}

void house_room_handler::turnOffBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->off([](const std::string &name)
                                      { useful_F::myStaticCtx->mqttHandler->publish(m_mqttPublishTopic, name); });
}

void house_room_handler::turnChangeBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->change([](const std::string &name)
                                         { useful_F::myStaticCtx->mqttHandler->publish(m_mqttPublishTopic, name); });
}

void house_room_handler::lockAllRoom()
{
    //  context->mqttHandler->publish("lkoko","kokok");
}

void house_room_handler::unlockAllRoom()
{
    //    for(auto& a : m_roomMap){
    //        value->unlock();
    //    }
}

nlohmann::json house_room_handler::getAllInfoJSON()
{
    nlohmann::json jj;
#ifdef BT_TEST
    std::cout << "mapa m_lightingBulbMap size: " << m_lightingBulbMap.size() << std::endl;
#endif

    for (auto &[key, value] : m_lightingBulbMap)
    {
        nlohmann::json roomJJ;
        roomJJ["STATE"] = stateToString(value->getLockState());
        roomJJ["STATUS"] = stateToString(value->getStatus());
        roomJJ["room"] = value->getRoomName();
        roomJJ["bulb ID"] = value->getID();
        roomJJ["bulb name"] = value->getBulbName();
        roomJJ["bulb counter"] = value->getBulbCounter();
        roomJJ["switch"] = value->getBulbPin();
        roomJJ["last working time"] = value->howLongBulbOn().getString();
        roomJJ["satelAlarm"] = stateToString(value->m_satelAlarm);
        roomJJ["howLong"] = value->m_satelAlarmHowLong;
        roomJJ["satelSensorAlarmUnixTime"] = Clock::unixTimeToString(value->getSatelSensorAlarmUnixTime());
        try
        {
            roomJJ["satelCounter"] = m_roomMap.at(value->getRoomName())->satelSensorCounter;
        }
        catch (...)
        {
            roomJJ["satelCounter"] = -1;
            log_file_mutex.mutex_lock();
            log_file_cout << ERROR << "sprawdz konfig pokojow! " << value->getRoomName() << " nie istnieje" << std::endl;
            log_file_mutex.mutex_unlock();
        }
        jj.push_back(roomJJ);
    }
    return jj;
}

nlohmann::json house_room_handler::getInfoJSON_allON()
{
    nlohmann::json jj;

#ifdef BT_TEST
    std::cout << "mapa m_lightingBulbMap size: " << m_lightingBulbMap.size() << std::endl;
#endif

    for (auto &[key, value] : m_lightingBulbMap)
    {
        if (value->getStatus() == STATE::ON or value->getStatus() == STATE::ACTIVE)
        {
            nlohmann::json roomJJ;
            roomJJ["STATE"] = stateToString(value->getLockState());
            roomJJ["STATUS"] = stateToString(value->getStatus());
            roomJJ["room"] = value->getRoomName();
            roomJJ["bulb ID"] = value->getID();
            roomJJ["bulb name"] = value->getBulbName();
            roomJJ["bulb counter"] = value->getBulbCounter();
            roomJJ["switch"] = value->getBulbPin();
            roomJJ["last working time"] = value->howLongBulbOn().getString();
            roomJJ["satelSensorAlarmUnixTime"] = Clock::unixTimeToString(value->getSatelSensorAlarmUnixTime());
            try
            {
                roomJJ["satelCounter"] = m_roomMap.at(value->getRoomName())->satelSensorCounter;
            }
            catch (...)
            {
                roomJJ["satelCounter"] = -1;
                log_file_mutex.mutex_lock();
                log_file_cout << ERROR << "sprawdz konfig pokojow! " << value->getRoomName() << " nie istnieje" << std::endl;
                log_file_mutex.mutex_unlock();
            }
            jj.push_back(roomJJ);
        }
    }
    return jj;
}

void house_room_handler::executeCommandFromMQTT(const std::string &msg)
{
    try
    {
        auto vv = useful_F::split(msg, ';');
        int bulbID = std::stoi(vv.at(1));
        int bulbState = std::stoi(vv.at(3));
        // DingDong  dzownek
        if (bulbID == 88 and bulbState == 1)
        {
            context->main_iDomTools->doorbellDingDong();
            useful_F_libs::httpPost("http://cyniu88.no-ip.pl/cgi-bin/dzwonek.sh", 10);
            // context->main_iDomTools->sendViberPicture("DZWONEK do drzwi!",
            //                                           "https://cyniu88.no-ip.pl/dzwonek/wejscie.jpg",
            //                                           context->server_settings->_fb_viber.viberReceiver.at(0),
            //                                           context->server_settings->_fb_viber.viberSender); // inform  door bell has been pressed

            // context->main_iDomTools->sendViberPicture("wjazd",
            //                                           "https://cyniu88.no-ip.pl/dzwonek/wjazd.jpg",
            //                                           context->server_settings->_fb_viber.viberReceiver.at(0),
            //                                           context->server_settings->_fb_viber.viberSender); // inform  door bell has been pressed
        }
        if (vv.at(0) == "state")
        {
            if (not m_lightingBulbMap.contains(bulbID))
            {
                m_lightingBulbMap.emplace(bulbID, std::make_shared<light_bulb>("roomName", "bulbName", bulbID));
            }

            STATE state;
            bool bState;

            if (bulbState == 1)
            {
                state = STATE::ON;
                bState = true;
            }
            else
            {
                state = STATE::OFF;
                bState = false;
            }

            m_lightingBulbMap.at(bulbID)->setStatus(state);
            // set state
            std::string name = m_lightingBulbMap.at(bulbID)->getRoomName();
            name.append("_");
            name.append(m_lightingBulbMap.at(bulbID)->getBulbName());

            // put info about bulb
            m_bulbStatus.Put({name, bState, Clock::getTimestamp()});

            context->main_iDomStatus->setObjectState(name, state);

            // TODO temporary added viber notifiction
            auto time = Clock::getUnixTime() - m_lastNotifyUnixTime;
            if (context->idom_all_state.alarmSatelState == STATE::ARMED or
                time > 600)
            {
                m_lastNotifyUnixTime += time;
                std::stringstream str_buf;
                str_buf << "zmiana statusu lampy " << m_lightingBulbMap.at(bulbID)->getBulbName()
                        << " w pomieszczeniu: " << m_lightingBulbMap.at(bulbID)->getRoomName()
                        << " na " << state
                        << " przyciskiem: " << vv.at(2)
                        << " czas trwania: " << m_lightingBulbMap.at(bulbID)->howLongBulbOn().getString();
                context->main_iDomTools->sendViberMsg(str_buf.str(), context->server_settings->_fb_viber.viberReceiver.at(0),
                                                      context->server_settings->_fb_viber.viberSender + "-light");
            }
        }
    }
    catch (...)
    {
        std::stringstream ret;
        ret << "bład odbioru mqtt light: " << msg;
        context->iDomAlarm.raiseAlarm(122333, ret.str());

        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << ret.str() << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void house_room_handler::executeButtonComand(const unsigned int buttonID,
                                             const std::string &action,
                                             CommandHandlerMQTT *commandMQTTptr)
{
    if (not m_buttonConfig.contains(buttonID))
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "unconfgured button: " << buttonID
                      << " action: " << action << std::endl;
        log_file_mutex.mutex_unlock();
        return; // button not used
    }
    if (not m_buttonConfig.at(buttonID).contains(action))
        return; // action not used
    for (const auto &element : m_buttonConfig.at(buttonID).find(action)->second)
    {
        auto v = useful_F::split(element, ' ');
        commandMQTTptr->run(v, context);
    }
}

void house_room_handler::satelSensorActive(int sensorID)
{
    if (sensorID == 1 or sensorID == 10 or sensorID == 3 or sensorID == 2)
    {
        return;
    }
    if (not m_satelIdMap.contains(sensorID))
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "unsupported  satel sensor " << sensorID << std::endl;
        log_file_cout << WARNING << "restart satel connections " << std::endl;
        log_file_mutex.mutex_unlock();
        context->main_iDomTools->sendViberMsg("restart polaczenia satel",
                                              context->server_settings->_fb_viber.viberReceiver.at(0),
                                              context->server_settings->_fb_viber.viberSender + "SATEL");
        context->satelIntegraHandler->getSatelPTR()->reconnectIntegra();

        return;
    }

    m_satelIdMap.at(sensorID)->satelSensorActive();
    m_circBuffSatelSensorId.put(sensorID);
}

void house_room_handler::turnOffUnexpectedBulb()
{
    auto time = Clock::getUnixTime();

    for (const auto &jj : m_lightingBulbMap)
    {
        if (jj.second->m_satelAlarmHowLong not_eq -1 && jj.second->getStatus() == STATE::ON)
        {
            auto actualTime = time - jj.second->getSatelSensorAlarmUnixTime();
            unsigned int expectTime = static_cast<unsigned int>(jj.second->m_satelAlarmHowLong * 60);
            if (actualTime > expectTime and jj.second->lighting_priority == false)
            {
                jj.second->off([](const std::string &name)
                               { useful_F::myStaticCtx->mqttHandler->publish(m_mqttPublishTopic, name); });
            }
        }
    }
}

std::string house_room_handler::dump() const
{
    std::stringstream str;

    for (auto &[key, value] : m_lightingBulbMap)
        str << "light " << key << " name " << value->getBulbName() << std::endl;

    str << "m_mqttPublishTopic: " << m_mqttPublishTopic << std::endl;
    str << "m_lastNotifyUnixTime: " << m_lastNotifyUnixTime << std::endl;
    str << "m_buttonConfig: " << m_buttonConfig.size() << std::endl;
    str << "m_circBuffSatelSensorId: " << std::endl
        << m_circBuffSatelSensorId.dump<int>() << std::endl;

    for (auto &[key, value] : m_satelIdMap)
        str << "m_satelIdMap " << key << " name " << value->m_name << std::endl;
    return str.str();
}
