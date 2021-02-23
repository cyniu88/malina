#ifndef ROOM_H
#define ROOM_H
#include <iostream>
#include <map>

#include "light_bulb.h"

class ROOM
{
public:
    int m_satelSensorID;
    std::string m_name;
    std::map<int, std::shared_ptr<light_bulb>> m_lightingBulbMap;
    STATE m_state = STATE::UNDEFINE;

    ROOM(int satelSensorID, std::string name, std::map<int, std::shared_ptr<light_bulb>> lightingBulbMap );

    void satelSensorActive();
};

#endif // ROOM_H
