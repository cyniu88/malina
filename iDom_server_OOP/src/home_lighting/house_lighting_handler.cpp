#include "house_lighting_handler.h"

house_lighting_handler::house_lighting_handler()
{

}

void house_lighting_handler::addRoom(std::string &name)
{
    m_roomMap[name] = std::make_unique<house_room>(name);
}

void house_lighting_handler::addBulbInRoom(std::string &roomName, std::string &bulbName, int bulbID)
{
    m_roomMap[roomName]->addBulb(bulbName,bulbID);
}

std::string house_lighting_handler::dump() const
{
    std::stringstream str;
    return str.str();
}
