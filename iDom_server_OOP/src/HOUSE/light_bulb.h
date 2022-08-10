#ifndef LIGHT_BULB_H
#define LIGHT_BULB_H

#include <mutex>
#include <functional>

#include "../idom_api.h"
#include "../../libs/useful/useful.h"

class light_bulb: public iDom_API
{
    STATE m_status = STATE::UNDEFINE;
    std::string m_roomName;
    std::string m_bulbName;
    int m_ID;
    std::mutex m_operationMutex;
    STATE m_lock = STATE::UNLOCK;
    std::vector<int> m_pins;
    Clock m_onTime;
    Clock m_offTime;
    unsigned int m_satelAlarmUnixTime = 0;
    unsigned int m_bulbCounter = 0;

public:
    light_bulb(const std::string& roomName, const std::string& bulbName, int id);
    ~light_bulb();
    light_bulb(const light_bulb& a);
    light_bulb(const light_bulb&& a);
    light_bulb& operator = (const light_bulb& a);
    light_bulb& operator = (light_bulb &&a);

    void on(std::function<void(std::string s)> onOn);
    void off(std::function<void(std::string s)> onOff);
    void change(std::function<void(std::string s)> changeF);

    STATE getStatus();
    void setStatus(STATE s);

    std::string getBulbName() const;
    std::string getRoomName() const;
    int getID() const;

    void lock();
    void unlock();

    STATE getLockState() const;

    void addBulbPin(int i);
    std::string getBulbPin();
    unsigned int getBulbCounter();
    nlohmann::json getStatsJSON();

    Clock howLongBulbOn();

    void satelSensorAlarm();
    unsigned int getSatelSensorAlarmUnixTime();

    std::string dump() const override;

    STATE m_onLock = STATE::UNDEFINE;
    STATE m_onUnlock = STATE::UNDEFINE;
    STATE m_onSunset = STATE::UNDEFINE;
    STATE m_onSunrise = STATE::UNDEFINE;
    STATE m_satelAlarm = STATE::UNDEFINE;
    int m_satelAlarmHowLong = -1;
};

#endif // LIGHT_BULB_H
