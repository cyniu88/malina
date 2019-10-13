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

    house_lighting_handler();
    void addRoom(std::string& name);
    void addBulbInRoom(std::string& roomName, std::string& bulbName, int bulbID);
    std::string dump() const;
};

#endif // HOUSE_LIGHTING_HANDLER_H
