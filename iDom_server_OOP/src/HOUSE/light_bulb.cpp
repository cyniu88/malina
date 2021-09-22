#include "light_bulb.h"

light_bulb::light_bulb(const std::string& roomName, const std::string &bulbName, int id):
    m_roomName(roomName),
    m_bulbName(bulbName),
    m_ID(id),
    m_onTime(Clock::getTime()),
    m_offTime(m_onTime)
{
#ifdef BT_TEST
    std::cout << "light_bulb::light_bulb()" << std::endl;
#endif
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
}

light_bulb::~light_bulb()
{
#ifdef BT_TEST
    std::cout << "~light_bulb::light_bulb()" << std::endl;
#endif
    iDom_API::removeFromMap(m_className);
}

light_bulb::light_bulb(const light_bulb &a):
    m_status(a.m_status),
    m_bulbName(a.m_bulbName),
    m_ID(a.m_ID),
    m_onTime(a.m_onTime),
    m_satelAlarmUnixTime(a.m_satelAlarmUnixTime),
    m_bulbCounter(a.m_bulbCounter),
    m_onLock(a.m_onLock),
    m_onUnlock(a.m_onUnlock),
    m_onSunset(a.m_onSunset),
    m_onSunrise(a.m_onSunrise)
{
#ifdef BT_TEST
    std::cout << "light_bulb::light_bulb(&)" << std::endl;
#endif
}

light_bulb::light_bulb(const light_bulb &&a)

{
#ifdef BT_TEST
    std::cout << "light_bulb::light_bulb(&&)" << std::endl;
#endif
    assert(this not_eq &a);
    *this = std::move(a);
}

light_bulb &light_bulb::operator=(const light_bulb &a)
{
#ifdef BT_TEST
    std::cout << "operator=(&)" << std::endl;
#endif
    m_status = a.m_status;
    m_ID = a.m_ID;
    m_bulbName = a.m_bulbName;
    m_onLock = a.m_onLock;
    m_onUnlock = a.m_onUnlock;
    m_onSunset = a.m_onSunset;
    m_onSunrise = a.m_onSunrise;
    m_onTime = a.m_onTime;
    m_satelAlarmUnixTime = a.m_satelAlarmUnixTime;
    m_bulbCounter = a.m_bulbCounter;
    return *this;
}

light_bulb& light_bulb::operator =(light_bulb&& a)
{
#ifdef BT_TEST
    std::cout << "operator=(&&)" << std::endl;
#endif
    assert(this not_eq &a);
    m_status = std::move(a.m_status);
    m_bulbName = std::move(a.m_bulbName);
    m_ID = std::move(a.m_ID);
    m_onLock = std::move(a.m_onLock);
    m_onUnlock = std::move(a.m_onUnlock);
    m_onSunset = std::move(a.m_onSunset);
    m_onSunrise = std::move(a.m_onSunrise);
    m_onTime = std::move(a.m_onTime);
    m_satelAlarmUnixTime = std::move(a.m_satelAlarmUnixTime);
    m_bulbCounter = std::move(a.m_bulbCounter);
    return *this;
}

void light_bulb::on(std::function<void(std::string s)>onOn)
{
    if(m_lock not_eq STATE::UNLOCK)
        return;
    std::lock_guard<std::mutex> lock (m_operationMutex);
    std::stringstream ss;
    ss << "0;" << m_ID << ";1;0;2;1";
    onOn(ss.str());
    m_status = STATE::ACTIVE;
}

void light_bulb::off(std::function<void(std::string s)> onOff)
{
    if(m_lock not_eq STATE::UNLOCK)
        return;
    std::lock_guard<std::mutex> lock (m_operationMutex);
    std::stringstream ss;
    ss << "0;" << m_ID << ";1;0;2;0";
    onOff(ss.str());
    m_status = STATE::DEACTIVE;
}

STATE light_bulb::getStatus()
{
    std::lock_guard<std::mutex> lock (m_operationMutex);
    return m_status;
}

void light_bulb::setStatus(STATE s)
{
    std::lock_guard<std::mutex> lock (m_operationMutex);
    if(s == STATE::ON){
        m_onTime = Clock::getTime();
        satelSensorAlarm(); //simulating motion sensor activation so as not to turn off the light immediately in the absence of motion
        ++m_bulbCounter;
    }
    else if(s == STATE::OFF){
        m_offTime = Clock::getTime();
    }

    m_status = s;
}

std::string light_bulb::getBulbName() const
{
    return m_bulbName;
}

std::string light_bulb::getRoomName() const
{
    return m_roomName;
}

int light_bulb::getID() const
{
    return m_ID;
}

void light_bulb::lock()
{
    m_lock = STATE::LOCK;
}

void light_bulb::unlock()
{
    m_lock = STATE::UNLOCK;
}

STATE light_bulb::getLockState() const
{
    return m_lock;
}

void light_bulb::addBulbPin(int i)
{
    m_pins.push_back(i);
}

std::string light_bulb::getBulbPin()
{
    std::stringstream str;
    for(const auto& a : m_pins){
        str << a <<",";
    }
    return str.str();
}

unsigned int light_bulb::getBulbCounter()
{
    return m_bulbCounter;
}

nlohmann::json light_bulb::getStatsJSON()
{
    nlohmann::json jj;

    jj["bulb name"] = m_bulbName;
    jj["counter"] = m_bulbCounter;
    jj["state"] = stateToString(m_status);

    return jj;
}

Clock light_bulb::howLongBulbOn()
{
    Clock ret(0,0);
    if(m_status == STATE::OFF)
        ret = Clock::periodOfTime(m_onTime, m_offTime);
    return ret;
}

void light_bulb::satelSensorAlarm()
{
    m_satelAlarmUnixTime = Clock::getUnixTime();
}

unsigned int light_bulb::getSatelSensorAlarmUnixTime()
{
    return m_satelAlarmUnixTime;
}

std::string light_bulb::dump() const
{
    std::stringstream str;
    str << "bulb name: " << m_bulbName << std::endl;
    str << "bulb ID: " << m_ID << std::endl;
    str << "bulb status: " << m_status << std::endl;
    str << "bulb in room: " << m_roomName << std::endl;
    str << "switch pin: ";
    for(const auto& a : m_pins){
        str << a <<",";
    }
    str << std::endl;
    str << "bulb m_onTime: " << m_onTime.getString() << std::endl;
    str << "bulb m_offTime: " << m_offTime.getString() << std::endl;
    str << "bulb m_onLock: " << m_onLock << std::endl;
    str << "bulb m_onUnlock: " << m_onUnlock << std::endl;
    str << "bulb m_onSunrise: " << m_onSunrise << std::endl;
    str << "bulb m_onSunset: " << m_onSunset << std::endl;
    str << "bulb m_satelAlarmUnixTime: " << m_satelAlarmUnixTime << std::endl;
    str << "m_bulbCounter: " << m_bulbCounter << std::endl;
    return str.str();
}
