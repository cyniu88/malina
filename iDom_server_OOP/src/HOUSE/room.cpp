#include "room.h"
#include "house_room_handler.h"
#include "../functions/functions.h"
#include "../iDomTools/idomtools_interface.h"

ROOM::ROOM(const std::vector<int> &satelSensorID, const std::string &name, const std::map<int, std::shared_ptr<light_bulb>> &lightingBulbMap) : m_satelSensorID(satelSensorID),
                                                                                                                                                m_name(name),
                                                                                                                                                m_lightingBulbMap(lightingBulbMap)
{
#ifdef BT_TEST
    std::cout << "ROOM::ROOM() " << name << std::endl;
#endif
}

void ROOM::satelSensorActive()
{
    ++satelSensorCounter;

    if (useful_F::myStaticCtx->idom_all_state.houseState == STATE::LOCK)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "ALARM !  w pokoju " << m_name << std::endl;
        log_file_mutex.mutex_unlock();

        if (useful_F::myStaticCtx->idom_all_state.counter++ < 10)
        {
            useful_F::myStaticCtx->main_iDomTools->sendViberMsg("alarm w pokoju " + m_name,
                                                                 useful_F::myStaticCtx->server_settings->_fb_viber.viberReceiver.at(0),
                                                                 "Satel Alarm");
        }
    }
    for (const auto &bulb : m_lightingBulbMap)
    {
        bulb.second->satelSensorAlarm();
        if (bulb.second->m_satelAlarm == STATE::ACTIVE &&
            useful_F::myStaticCtx->main_iDomTools->isItDay() == false &&
            bulb.second->getStatus() not_eq STATE::ON)
        {
            bulb.second->on([](const std::string &name)
                            { useful_F::myStaticCtx->mqttHandler->publish(house_room_handler::m_mqttPublishTopic, name); });
        } // if
    }     // for
}

nlohmann::json ROOM::getStatsJSON()
{
    nlohmann::json jj, kk;
    jj["room name"] = m_name;
    jj["satel activatet counter"] = satelSensorCounter;

    std::transform(m_lightingBulbMap.cbegin(),
                   m_lightingBulbMap.cend(),
                   std::back_inserter(kk),
                   [](std::pair<int, std::shared_ptr<light_bulb>> c)
                   { return c.second->getStatsJSON(); });

    jj["z bulb"] = kk;
    // jj.push_back(kk);
    return jj;
}
