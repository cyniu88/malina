#include "house_lighting_handler.h"
#include "json.hpp"
#include "../functions/functions.h"
#include <fstream>

house_lighting_handler::house_lighting_handler(std::string &configPath)
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);

    std::ifstream i(configPath);
    nlohmann::json j;
    i >> j;
    for (auto& element : j) {
        std::string roomName = element.at("room").get<std::string>();
        std::string bulbName = element.at("bulbName").get<std::string>();
        int bulbID = element.at("bulbID").get<int>();

        m_lightingBulbMap.emplace(bulbID, std::make_shared<light_bulb>(roomName, bulbName, bulbID));

        m_roomMap[roomName].push_back(m_lightingBulbMap[bulbID]);
    }
}

house_lighting_handler::~house_lighting_handler()
{
    iDom_API::removeFromMap(m_className);
}


void house_lighting_handler::turnOnAllInRoom(const std::string &roomName)
{
    //    m_roomMap[roomName]->allOn([](const std::string& name){
    //        useful_F::myStaticData->mqttHandler->publish("test topick",name);
    //    }
    //                               );

    for( auto& a :m_roomMap[roomName])
    {
        a->on([](const std::string& name){
           useful_F::myStaticData->mqttHandler->publish("test topick",name);
    }
    );
    }
}

void house_lighting_handler::turnOffAllInRoom(const std::string &roomName)
{
    //    m_roomMap[roomName]->allOff([](const std::string& name){
    //        useful_F::myStaticData->mqttHandler->publish("test topick",name);
    //    }
    //                                );

    for( auto& a :m_roomMap[roomName])
    {
        a->off([](const std::string& name){
            useful_F::myStaticData->mqttHandler->publish("test topick",name);
        }
              );
    }

}

void house_lighting_handler::turnOnBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->on([](const std::string& name){
        useful_F::myStaticData->mqttHandler->publish("test topick",name);
    }
                                         );
}

void house_lighting_handler::turnOffBulb(const int bulbID)
{
    m_lightingBulbMap.at(bulbID)->off([](const std::string& name){
        useful_F::myStaticData->mqttHandler->publish("test topick",name);
    }
                                     );
}


void house_lighting_handler::lockAllRoom()
{
    //    for(auto& a : m_roomMap){
    //        a.second->lock();
    //    }
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

    for(auto&a : m_lightingBulbMap){
        nlohmann::json roomJJ;

        roomJJ["STATE"] = stateToString(a.second->getLockState());
        roomJJ["STATUS"] = stateToString(a.second->getStatus());
        roomJJ["room"] = a.second->getRoomName();
        jj[a.second->getBulbName()] = roomJJ;
    }

    return jj;
}

std::string house_lighting_handler::dump() const
{
    std::stringstream str;

    for(auto& a : m_lightingBulbMap)
        str << "light " << a.first << " name " << a.second->getBulbName() << std::endl;

    return str.str();
}
