#ifndef HOUSE_LIGHTING_HANDLER_H
#define HOUSE_LIGHTING_HANDLER_H

#include <map>
#include <memory>

#include "../idom_api.h"
#include "json.hpp"
//#include "../iDom_server_OOP.h"
#include "light_bulb.h"
struct thread_data;
class house_lighting_handler: public iDom_API
{
    thread_data* my_data;
    static std::string  m_mqttPublishTopic;
    unsigned int m_lastNotifyUnixTime = 0;

public:
    std::map<int, std::shared_ptr<light_bulb>> m_lightingBulbMap;
    std::map<std::string, std::vector<std::shared_ptr<light_bulb>> > m_roomMap;

    explicit house_lighting_handler(thread_data *my_data);
    ~house_lighting_handler();

    void loadConfig(std::string& configPath);

    void turnOnAllInRoom(const std::string& roomName);
    void turnOffAllInRoom(const std::string& roomName);

    void turnOnAllBulb();
    void turnOffAllBulb();

    void turnOnBulb(const int bulbID);
    void turnOffBulb(const int bulbID);

    void lockAllRoom();
    void unlockAllRoom();

    nlohmann::json getAllInfoJSON();
    nlohmann::json getInfoJSON_allON();

    void executeCommandFromMQTT(std::string& msg);

    std::string dump() const override;
};

#endif // HOUSE_LIGHTING_HANDLER_H
