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
public:
    light_bulb(const std::string& roomName, const std::string& bulbName, int id);
    ~light_bulb();
    light_bulb(const light_bulb& a);
    light_bulb(const light_bulb&& a);
    light_bulb& operator = (const light_bulb& a);
    light_bulb& operator = (light_bulb &&a);

    void on(std::function<void(std::string s)>onOn);
    void off(std::function<void(std::string s)>onOff);

    STATE getStatus();
    void setStatus(STATE s);

    std::string getBulbName() const;
    std::string getRoomName() const;
    int getID() const;

    void lock();
    void unlock();

    STATE getLockState() const;

    std::string dump() const override;
};

#endif // LIGHT_BULB_H
