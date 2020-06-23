#include "house_lighting_handler.h"
#include "json.hpp"
#include "../functions/functions.h"
#include <fstream>

std::string house_lighting_handler::m_mqttPublishTopic = "swiatlo/output/";

house_lighting_handler::house_lighting_handler(thread_data *my_data){
    m_className.append(typeid (this).name());
    this->my_data =  my_data;
    iDom_API::addToMap(m_className,this);
}

house_lighting_handler::~house_lighting_handler()
{
    iDom_API::removeFromMap(m_className);
}

void house_lighting_handler::loadConfig(std::string &configPath)
{
    std::ifstream i(configPath);
    nlohmann::json j;
    i >> j;
    for (auto& element : j) {
        std::string roomName = element.at("room").get<std::string>();
        std::string bulbName = element.at("bulbName").get<std::string>();
        int bulbID = element.at("bulbID").get<int>();

        m_lightingBulbMap.emplace(bulbID, std::make_shared<light_bulb>(roomName, bulbName, bulbID));

        for (const auto& jj :  element.at("switchID"))
        {
            m_lightingBulbMap.at(bulbID)->addBulbPin(jj.get<int>());
        }

        m_roomMap[roomName].push_back(m_lightingBulbMap[bulbID]);
    }
}


void house_lighting_handler::turnOnAllInRoom(const std::string &roomName)
{
    for( auto& a :m_roomMap[roomName])
    {
        a->on([](const std::string& name){
            useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic,name);
        }
              );
    }
}

void house_lighting_handler::turnOffAllInRoom(const std::string &roomName)
{
    for( auto& a :m_roomMap[roomName])
    {
        a->off([](const std::string& name){
            useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic,name);
        }
               );
    }
}

void house_lighting_handler::turnOnAllBulb()
{
    for(auto& b : m_roomMap){
        turnOnAllInRoom(b.second.at(0)->getRoomName());
    }
}

void house_lighting_handler::turnOffAllBulb()
{
    for(auto& b : m_roomMap){
        turnOffAllInRoom(b.second.at(0)->getRoomName());
    }
}

void house_lighting_handler::turnOnBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->on([](const std::string& name){
        useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic,name);
    }
                                     );
}

void house_lighting_handler::turnOffBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->off([](const std::string& name){
        useful_F::myStaticData->mqttHandler->publish(m_mqttPublishTopic,name);
    }
                                      );
}


void house_lighting_handler::lockAllRoom()
{
    //  my_data->mqttHandler->publish("lkoko","kokok");
}

void house_lighting_handler::unlockAllRoom()
{
    //    for(auto& a : m_roomMap){
    //        a.second->unlock();
    //    }
}

nlohmann::json house_lighting_handler::getAllInfoJSON()
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
        roomJJ["bubl name"] = a.second->getBulbName();
        roomJJ["switch"] = a.second->getBulbPin();
        roomJJ["working time"] = a.second->howLongBulbOn().getString();
        jj.push_back(roomJJ);//[a.second->getID()] = roomJJ;
    }
    return jj;
}

nlohmann::json house_lighting_handler::getInfoJSON_allON()
{
    nlohmann::json jj;
   // jj["all"] = "ON";
#ifdef BT_TEST
    std::cout << "mapa m_lightingBulbMap size: " << m_lightingBulbMap.size() << std::endl;
#endif

    for(auto&a : m_lightingBulbMap){
        if(a.second->getStatus() == STATE::ON)
        {
            nlohmann::json roomJJ;
            roomJJ["STATE"] = stateToString(a.second->getLockState());
            roomJJ["STATUS"] = stateToString(a.second->getStatus());
            roomJJ["room"] = a.second->getRoomName();
            roomJJ["bulb ID"] = a.second->getID();
            roomJJ["bubl name"] = a.second->getBulbName();
            roomJJ["switch"] = a.second->getBulbPin();
            jj.push_back(roomJJ);//[a.second->getID()] = roomJJ;
        }
    }
    return jj;
}

void house_lighting_handler::executeCommandFromMQTT(std::string &msg)
{
    try {
        auto vv = useful_F::split(msg,';');
        int bulbID = std::stoi(vv.at(1));

        if(vv.at(0) == "state"){
            if(m_lightingBulbMap.find(bulbID) == m_lightingBulbMap.end()){
                m_lightingBulbMap.emplace(bulbID, std::make_shared<light_bulb>("roomName", "bulbName", bulbID));
            }

            STATE state;
            if(vv.at(2) == "-1"){
                if(vv.at(3) == "0")
                    state = STATE::ON;
                else
                    state = STATE::OFF;
            }
            else{
                if(vv.at(3) == "1")
                    state = STATE::ON;
                else
                    state = STATE::OFF;
            }

            m_lightingBulbMap.at(bulbID)->setStatus(state);
            //TODO temporary added viber notifiction
            std::stringstream str_buf;
            str_buf << "zmana statusu lampy " << bulbID
                    << " w pomieszczeniu: " << m_lightingBulbMap.at(bulbID)->getRoomName()
                    << " na " << stateToString(state)
                    << " przyciskiem: " << vv.at(2);
            my_data->main_iDomTools->sendViberMsg(str_buf.str(),my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                  my_data->server_settings->_fb_viber.viberSender + "-light");
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

std::string house_lighting_handler::dump() const
{
    std::stringstream str;

    for(auto& a : m_lightingBulbMap)
        str << "light " << a.first << " name " << a.second->getBulbName() << std::endl;

    str << "m_mqttPublishTopic: " << m_mqttPublishTopic << std::endl;
    return str.str();
}
