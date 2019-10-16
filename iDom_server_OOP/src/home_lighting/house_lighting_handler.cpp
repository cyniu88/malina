#include "house_lighting_handler.h"

house_lighting_handler::house_lighting_handler()
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
}

house_lighting_handler::~house_lighting_handler()
{
    iDom_API::removeFromMap(m_className);
}

void house_lighting_handler::addRoom(const std::string &name)
{
    m_roomMap[name] = std::make_unique<house_room>(name);
}

void house_lighting_handler::addBulbInRoom(const std::string &roomName, const std::string &bulbName, int bulbID)
{
    m_roomMap[roomName]->addBulb(bulbName,bulbID, &m_lightingBulbMap);
}

void house_lighting_handler::turnOnAllInRoom(const std::string &roomName)
{
    m_roomMap[roomName]->allOn([](std::string name){
        puts("test all on");
    }
                               );
}

void house_lighting_handler::turnOffAllInRoom(const std::string &roomName)
{
    m_roomMap[roomName]->allOff([](std::string name){
        puts("test all off");
    }
    );
}

void house_lighting_handler::lockAllRoom()
{
    for(auto& a : m_roomMap){
        a.second->lock();
    }
}

void house_lighting_handler::unlockAllRoom()
{
    for(auto& a : m_roomMap){
        a.second->unlock();
    }
}

nlohmann::json house_lighting_handler::getAllInfoJSON()
{
    nlohmann::json jj;

    for(auto&a : m_roomMap){
        nlohmann::json roomJJ;

        roomJJ["STATE"] = stateToString(a.second->getLockState());
        roomJJ["bulb"] = a.second->getJsonInfoLightBulb();
        jj[a.second->getName()] = roomJJ;
    }

    return jj;
}

std::string house_lighting_handler::dump() const
{
    std::stringstream str;

    for(auto& a : m_lightingBulbMap)
        str << "light " << a.first << " name " << a.second->getName() << std::endl;

    return str.str();
}
