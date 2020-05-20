#ifndef HOUSE_LIGHTING_HANDLER_H
#define HOUSE_LIGHTING_HANDLER_H

#include <map>
#include <memory>

#include "../idom_api.h"
#include "json.hpp"
#include "../iDom_server_OOP.h"
#include "light_bulb.h"

class house_lighting_handler: public iDom_API
{
public:
    std::map<int, std::shared_ptr<light_bulb>> m_lightingBulbMap;
    std::map<std::string, std::vector<std::shared_ptr<light_bulb>> > m_roomMap;

    house_lighting_handler(std::string& configPath);
    ~house_lighting_handler();

    void turnOnAllInRoom(const std::string& roomName);
    void turnOffAllInRoom(const std::string& roomName);

    void turnOnBulb(const int bulbID);
    void turnOffBulb(const int bulbID);

    void lockAllRoom();
    void unlockAllRoom();

    nlohmann::json getAllInfoJSON();

    std::string dump() const override;
};

#endif // HOUSE_LIGHTING_HANDLER_H
