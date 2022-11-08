#ifndef HOUSE_LIGHTING_HANDLER_H
#define HOUSE_LIGHTING_HANDLER_H

#include <map>
#include <memory>

#include "../idom_api.h"
#include "json.hpp"
#include "light_bulb.h"
#include "room.h"
#include "../libs/circular_buffer/circular_buffer.h"

class CommandHandlerMQTT;
struct thread_data;

class house_room_handler: public iDom_API
{
    thread_data* my_data;
    unsigned int m_lastNotifyUnixTime = 0;

    Circular_buffer m_circBuffSatelSensorId;

public:
    std::map<int, std::map<std::string, std::vector<std::string>>> m_buttonConfig;
    static std::string  m_mqttPublishTopic;

    std::map<int, std::shared_ptr<light_bulb>> m_lightingBulbMap;
    std::map<std::string, std::shared_ptr<ROOM>> m_roomMap;
    std::map<int, std::shared_ptr<ROOM>> m_satelIdMap;

    explicit house_room_handler(thread_data *my_data);
    ~house_room_handler();

    void loadConfig(const std::string& configPath);
    void loadButtonConfig(const std::string &configPath);

    void turnOnAllInRoom(const std::string& roomName);
    void turnOffAllInRoom(const std::string& roomName);
    void changeAllInRoom(const std::string& roomName);

    void turnOnAllBulb();
    void turnOffAllBulb();

    void turnOnBulb(const int bulbID);
    void turnOffBulb(const int bulbID);
    void turnChangeBulb(const int bulbID);

    void lockAllRoom();
    void unlockAllRoom();

    nlohmann::json getAllInfoJSON();
    nlohmann::json getInfoJSON_allON();

    void executeCommandFromMQTT(const std::string &msg);
    void executeButtonComand(const unsigned int buttonID, const std::string & action, CommandHandlerMQTT* commandMQTTptr);

    void satelSensorActive(int sensorID);

    void turnOffUnexpectedBulb();

    std::string dump() const override;
};

#endif // HOUSE_LIGHTING_HANDLER_H
