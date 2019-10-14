#ifndef HOUSE_LIGHTING_HANDLER_H
#define HOUSE_LIGHTING_HANDLER_H

#include <map>
#include <memory>

#include "../idom_api.h"
#include "house_room.h"

class house_lighting_handler: public iDom_API
{
public:
    std::map<std::string, std::unique_ptr<house_room>> m_roomMap;
    std::map<int, std::shared_ptr<light_bulb>> m_lightingBulbMap;

    house_lighting_handler();
    void addRoom(std::string& name);
    void addBulbInRoom(std::string& roomName, std::string& bulbName, int bulbID);

    void turnOnAllInRoom(std::string& roomName);

    std::string dump() const;
};

#endif // HOUSE_LIGHTING_HANDLER_H
