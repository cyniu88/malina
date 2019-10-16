#ifndef HOUSE_LIGHTING_HANDLER_H
#define HOUSE_LIGHTING_HANDLER_H

#include <map>
#include <memory>

#include "../idom_api.h"
#include "house_room.h"
#include "json.hpp"

class house_lighting_handler: public iDom_API
{
public:
    std::map<std::string, std::unique_ptr<house_room>> m_roomMap;
    std::map<int, std::shared_ptr<light_bulb>> m_lightingBulbMap;

    house_lighting_handler();
    ~house_lighting_handler();
    void addRoom(const std::string &name);
    void addBulbInRoom(const std::string& roomName, const std::string& bulbName, int bulbID);

    void turnOnAllInRoom(const std::string& roomName);
    void turnOffAllInRoom(const std::string& roomName);

    void lockAllRoom();
    void unlockAllRoom();

    nlohmann::json getAllInfoJSON();

    std::string dump() const;
};

#endif // HOUSE_LIGHTING_HANDLER_H
