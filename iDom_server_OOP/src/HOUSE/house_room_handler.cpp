#include <fstream>

#include "house_room_handler.h"
#include "json.hpp"
#include "../functions/functions.h"
#include "../RADIO_433_eq/radio_433_eq.h"
#include "../433MHz/RFLink/rflinkhandler.h"

std::string house_room_handler::m_mqttPublishTopic = "swiatlo/output/";

house_room_handler::house_room_handler(thread_data *my_data){
    m_className.append(typeid (this).name());
    this->my_data = my_data;
    iDom_API::addToMap(m_className,this);
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
    for (const auto& element : j) {
        std::string roomName = element.at("name").get<std::string>();
        int satelSensorID = element.at("satelSensorID").get<int>();
        std::map<int, std::shared_ptr<light_bulb>> lightingBulbMap;

        for (const auto& jj :  element.at("bulb"))
        {
            std::string bulbName = jj.at("bulbName").get<std::string>();
            int bulbID = jj.at("bulbID").get<int>();
            lightingBulbMap.emplace(bulbID, std::make_shared<light_bulb>(roomName, bulbName, bulbID));
            lightingBulbMap[bulbID]->m_onLock = stringToState(jj.at("lock").get<std::string>());
            lightingBulbMap[bulbID]->m_onUnlock = stringToState(jj.at("unlock").get<std::string>());
            lightingBulbMap[bulbID]->m_onSunrise = stringToState(jj.at("sunrise").get<std::string>());
            lightingBulbMap[bulbID]->m_onSunset = stringToState(jj.at("sunset").get<std::string>());
            lightingBulbMap[bulbID]->m_satelAlarm = stringToState(jj.at("satelAlarm").get<std::string>());
            lightingBulbMap[bulbID]->m_satelAlarmHowLong = jj.at("howLong").get<int>();

            for (const auto& kk :  jj.at("switchID"))
            {
                lightingBulbMap.at(bulbID)->addBulbPin(kk.get<int>());
            }
            if(m_lightingBulbMap.find(bulbID) == m_lightingBulbMap.end()){
                m_lightingBulbMap[bulbID] = lightingBulbMap[bulbID];
            }
            else{
                log_file_mutex.mutex_lock();
                log_file_cout << ERROR << "sprawdz konfig zarowek! "<< bulbID << " już istnieje" << std::endl;
                log_file_mutex.mutex_unlock();
            }
        }
        m_roomMap.emplace(roomName,std::make_shared<ROOM>(satelSensorID, roomName, lightingBulbMap));
        m_satelIdMap[satelSensorID] = m_roomMap[roomName];
    }
}

void house_room_handler::loadButtonConfig(const std::string &configPath)
{
    nlohmann::json jj;
    std::ifstream i(configPath);
    i >> jj;

    for(const auto& element : jj){
        auto buttonID = element.at("buttonID").get<int>();
        if(element.contains("long")){
            for(const auto& kk: element.at("long")){
                m_buttonConfig[buttonID]["long"].push_back(kk.get<std::string>());
            }
        }
        if(element.contains("double")){
            for(const auto& kk: element.at("double")){
                m_buttonConfig[buttonID]["double"].push_back(kk.get<std::string>());
            }
        }
    }
}

void house_room_handler::turnOnAllInRoom(const std::string &roomName)
{
    for( auto& a :m_roomMap[roomName]->m_lightingBulbMap)
    {
        a.second->on([](const std::string& name){
            useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic,name);
        }
        );
    }
}

void house_room_handler::turnOffAllInRoom(const std::string &roomName)
{
    for( auto& a :m_roomMap[roomName]->m_lightingBulbMap)
    {
        a.second->off([](const std::string& name){
            useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic,name);
        }
        );
    }
}

void house_room_handler::turnOnAllBulb()
{
    for(auto& b : m_roomMap){
        turnOnAllInRoom(b.second->m_name);
    }
}

void house_room_handler::turnOffAllBulb()
{
    for(auto& b : m_roomMap){
        turnOffAllInRoom(b.second->m_name);
    }
}

void house_room_handler::turnOnBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->on([](const std::string& name){
        useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic, name);
    }
    );
}

void house_room_handler::turnOffBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->off([](const std::string& name){
        useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic, name);
    }
    );
}

void house_room_handler::lockAllRoom()
{
    //  my_data->mqttHandler->publish("lkoko","kokok");
}

void house_room_handler::unlockAllRoom()
{
    //    for(auto& a : m_roomMap){
    //        a.second->unlock();
    //    }
}

nlohmann::json house_room_handler::getAllInfoJSON()
{
    nlohmann::json jj;
#ifdef BT_TEST
    std::cout << "mapa m_lightingBulbMap size: " << m_lightingBulbMap.size() << std::endl;
#endif

    for(auto&a : m_lightingBulbMap){
        nlohmann::json roomJJ;
        roomJJ["STATE"] = stateToString(a.second->getLockState());
        roomJJ["STATUS"] = stateToString(a.second->getStatus());
        roomJJ["room"] = a.second->getRoomName();
        roomJJ["bulb ID"] = a.second->getID();
        roomJJ["bulb name"] = a.second->getBulbName();
        roomJJ["bulb counter"] = a.second->getBulbCounter();
        roomJJ["switch"] = a.second->getBulbPin();
        roomJJ["last working time"] = a.second->howLongBulbOn().getString();
        roomJJ["lock"] = stateToString(a.second->m_onLock);
        roomJJ["unlock"] = stateToString(a.second->m_onUnlock);
        roomJJ["sunset"] = stateToString(a.second->m_onSunset);
        roomJJ["sunrise"] = stateToString(a.second->m_onSunrise);
        roomJJ["satelAlarm"] = stateToString(a.second->m_satelAlarm);
        roomJJ["howLong"] = a.second->m_satelAlarmHowLong;
        roomJJ["satelSensorAlarmUnixTime"] = Clock::unixTimeToString(a.second->getSatelSensorAlarmUnixTime());
        try {
            roomJJ["satelCounter"] = m_roomMap.at(a.second->getRoomName())->satelSensorCounter;
        }  catch (...) {
            roomJJ["satelCounter"] = -1;
            log_file_mutex.mutex_lock();
            log_file_cout << ERROR << "sprawdz konfig pokojow! "<< a.second->getRoomName() << " nie istnieje" << std::endl;
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

    for(auto&a : m_lightingBulbMap){
        if(a.second->getStatus() == STATE::ON or a.second->getStatus() == STATE::ACTIVE)
        {
            nlohmann::json roomJJ;
            roomJJ["STATE"] = stateToString(a.second->getLockState());
            roomJJ["STATUS"] = stateToString(a.second->getStatus());
            roomJJ["room"] = a.second->getRoomName();
            roomJJ["bulb ID"] = a.second->getID();
            roomJJ["bulb name"] = a.second->getBulbName();
            roomJJ["bulb counter"] = a.second->getBulbCounter();
            roomJJ["switch"] = a.second->getBulbPin();
            roomJJ["last working time"] = a.second->howLongBulbOn().getString();
            roomJJ["lock"] = stateToString(a.second->m_onLock);
            roomJJ["unlock"] = stateToString(a.second->m_onUnlock);
            roomJJ["sunset"] = stateToString(a.second->m_onSunset);
            roomJJ["sunrise"] = stateToString(a.second->m_onSunrise);
            roomJJ["satelSensorAlarmUnixTime"] = Clock::unixTimeToString(a.second->getSatelSensorAlarmUnixTime());
            try {
                roomJJ["satelCounter"] = m_roomMap.at(a.second->getRoomName())->satelSensorCounter;
            }  catch (...) {
                roomJJ["satelCounter"] = -1;
                log_file_mutex.mutex_lock();
                log_file_cout << ERROR << "sprawdz konfig pokojow! "<< a.second->getRoomName() << " nie istnieje" << std::endl;
                log_file_mutex.mutex_unlock();
            }
            jj.push_back(roomJJ);
        }
    }
    return jj;
}

void house_room_handler::executeCommandFromMQTT(const std::string &msg)
{
    try {
        auto vv = useful_F::split(msg,';');
        int bulbID = std::stoi(vv.at(1));
        // DingDong  dzownek
        if(bulbID == 88){
            my_data->main_iDomTools->doorbellDingDong();
            my_data->main_iDomTools->sendViberPicture("DZWONEK do drzwi!",
                                                      "https://c8.alamy.com/comp/2C2EMMC/hand-push-the-bell-button-at-the-front-door-finger-presses-the-doorbell-switch-person-rings-in-the-apartment-flat-vector-illustration-2C2EMMC.jpg",
                                                      my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                      my_data->server_settings->_fb_viber.viberSender);   // inform  door bell has been pressed

        }
        if(vv.at(0) == "state"){
            if(m_lightingBulbMap.find(bulbID) == m_lightingBulbMap.end()){
                m_lightingBulbMap.emplace(bulbID, std::make_shared<light_bulb>("roomName", "bulbName", bulbID));
            }

            STATE state;

            if(std::stoi(vv.at(3)) == 1)
                state = STATE::ON;
            else
                state = STATE::OFF;


            m_lightingBulbMap.at(bulbID)->setStatus(state);
            // set state
            std::string name = m_lightingBulbMap.at(bulbID)->getRoomName();
            name.append("_");
            name.append(m_lightingBulbMap.at(bulbID)->getBulbName());
            my_data->main_iDomStatus->setObjectState(name,state);

            // TODO temporary added viber notifiction
            auto time = Clock::getUnixTime() - m_lastNotifyUnixTime;
            if (my_data->idom_all_state.houseState == STATE::LOCK or
                    time > 60) {
                m_lastNotifyUnixTime += time;
                std::stringstream str_buf;
                str_buf << "zmiana statusu lampy " << m_lightingBulbMap.at(bulbID)->getBulbName()
                        << " w pomieszczeniu: " << m_lightingBulbMap.at(bulbID)->getRoomName()
                        << " na " << state
                        << " przyciskiem: " << vv.at(2)
                        << " czas trwania: " <<  m_lightingBulbMap.at(bulbID)->howLongBulbOn().getString();
                my_data->main_iDomTools->sendViberMsg(str_buf.str(),my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                      my_data->server_settings->_fb_viber.viberSender + "-light");
            }
        }

    } catch (...) {
        std::stringstream ret;
        ret << "bład odbioru mqtt light: " << msg;
        my_data->iDomAlarm.raiseAlarm(122333,ret.str());

        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << ret.str() << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void house_room_handler::executeButtonComand(const unsigned int buttonID, const std::string &action, CommandHandlerMQTT *commandMQTTptr)
{
    if(m_buttonConfig.find(buttonID) == m_buttonConfig.end())
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "unconfgured button "  << buttonID << std::endl;
        log_file_mutex.mutex_unlock();
        return; // button not used
    }
    if(m_buttonConfig.at(buttonID).find(action) == m_buttonConfig.at(buttonID).end())
        return; // action not used
    for(const auto& element : m_buttonConfig.at(buttonID).find(action)->second){
        auto v = useful_F::split(element, ' ');
         commandMQTTptr->run(v, my_data);
    }
}

void house_room_handler::onLock()
{
    for(const auto &  jj : m_lightingBulbMap){
        if(jj.second->m_onLock == STATE::ON)
            turnOnBulb(jj.first);
        else if(jj.second->m_onLock == STATE::OFF)
            turnOffBulb(jj.first);
    }
}

void house_room_handler::onUnlock()
{
    for(const auto &  jj : m_lightingBulbMap){
        if(jj.second->m_onUnlock == STATE::ON)
            turnOnBulb(jj.first);
        else if(jj.second->m_onUnlock == STATE::OFF)
            turnOffBulb(jj.first);
    }
}

void house_room_handler::onSunset()
{
    for(const auto &  jj : m_lightingBulbMap){
        if(jj.second->m_onSunset == STATE::ON)
            turnOnBulb(jj.first);
        else if(jj.second->m_onSunset == STATE::OFF)
            turnOffBulb(jj.first);
    }
}

void house_room_handler::onSunrise()
{
    for(const auto &  jj : m_lightingBulbMap){
        if(jj.second->m_onSunrise == STATE::ON)
            turnOnBulb(jj.first);
        else if(jj.second->m_onSunrise == STATE::OFF)
            turnOffBulb(jj.first);
    }
}

void house_room_handler::satelSensorActive(int sensorID)
{
    if(m_satelIdMap.find(sensorID) == m_satelIdMap.end() ) {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "unsupported  satel sensor " << sensorID << std::endl;
        log_file_mutex.mutex_unlock();
        return;
    }
    m_satelIdMap.at(sensorID)->satelSensorActive();
    m_circBuffSatelSensorId.put(sensorID);
}

void house_room_handler::turnOffUnexpectedBulb()
{
    auto time = Clock::getUnixTime();

    for(const auto &  jj : m_lightingBulbMap){
        if(jj.second->m_satelAlarmHowLong not_eq -1 && jj.second->getStatus() == STATE::ON){
            auto actualTime = time - jj.second->getSatelSensorAlarmUnixTime();
            unsigned int expectTime = static_cast<unsigned int >(jj.second->m_satelAlarmHowLong * 60);
            if( actualTime > expectTime)
            {
                jj.second->off([](const std::string& name){
                    useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic,name);
                });
            }
        }
    }
}

std::string house_room_handler::dump() const
{
    std::stringstream str;

    for(auto& a : m_lightingBulbMap)
        str << "light " << a.first << " name " << a.second->getBulbName() << std::endl;

    str << "m_mqttPublishTopic: " << m_mqttPublishTopic << std::endl;
    str << "m_lastNotifyUnixTime: " << m_lastNotifyUnixTime << std::endl;
    str << "m_buttonConfig: " << m_buttonConfig.size() << std::endl;
    str << "m_circBuffSatelSensorId: " << std::endl << m_circBuffSatelSensorId.dump<int>() << std::endl;


    return str.str();
}
