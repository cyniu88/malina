#include "room.h"
#include "house_room_handler.h"
#include "../functions/functions.h"

ROOM::ROOM(int satelSensorID, std::string name, std::map<int, std::shared_ptr<light_bulb> > lightingBulbMap):
    m_satelSensorID(satelSensorID),
    m_name(name),
    m_lightingBulbMap(lightingBulbMap)
{

}

void ROOM::satelSensorActive()
{
    for(const auto& bulb :m_lightingBulbMap){
        bulb.second->satelSensorAlarm();
        if(bulb.second->m_satelAlarm == STATE::ACTIVE &&
                useful_F::myStaticData->main_iDomTools->isItDay() == false &&
                bulb.second->getStatus() != STATE::ON ){
            bulb.second->on([](const std::string& name){
                useful_F::myStaticData->mqttHandler->publish(house_room_handler::m_mqttPublishTopic,name);
            }
            );
        }//if
    }//for
}
