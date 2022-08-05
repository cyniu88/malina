#ifndef ROOM_H
#define ROOM_H
#include <iostream>
#include <map>

#include "light_bulb.h"

class ROOM
{
public:
    std::vector<int> m_satelSensorID;
    unsigned int satelSensorCounter = 0;
    std::string m_name;
    std::map<int, std::shared_ptr<light_bulb>> m_lightingBulbMap;
    STATE m_state = STATE::UNDEFINE;

    ROOM(std::vector<int> satelSensorID, const std::string &name, const std::map<int, std::shared_ptr<light_bulb>> & lightingBulbMap );

    void satelSensorActive();
    nlohmann::json getStatsJSON();
};

#endif // ROOM_H
