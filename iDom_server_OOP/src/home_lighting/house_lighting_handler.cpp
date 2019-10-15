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

void house_lighting_handler::addRoom(std::string &name)
{
    m_roomMap[name] = std::make_unique<house_room>(name);
}

void house_lighting_handler::addBulbInRoom(std::string &roomName, std::string &bulbName, int bulbID)
{
    m_roomMap[roomName]->addBulb(bulbName,bulbID, &m_lightingBulbMap);
}

void house_lighting_handler::turnOnAllInRoom(std::string &roomName)
{
    m_roomMap[roomName]->allOn([](std::string name){
        puts("DUPA");
    }
                               );
}

std::string house_lighting_handler::dump() const
{
    std::stringstream str;

    for(auto& a : m_lightingBulbMap)
        str << "light " << a.first << " name " << a.second->getName() << std::endl;

    return str.str();
}
