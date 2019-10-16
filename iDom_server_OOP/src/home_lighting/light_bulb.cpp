#include "light_bulb.h"

light_bulb::light_bulb(const std::string &name, int id): m_name(name), m_ID(id)
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
    m_name(a.m_name),
    m_ID(a.m_ID)
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
    *this = std::move(a);
}

light_bulb &light_bulb::operator=(const light_bulb &a)
{
#ifdef BT_TEST
    std::cout << "operator=(&)" << std::endl;
#endif
    m_status = a.m_status;
    m_ID = a.m_ID;
    m_name = a.m_name;
    return *this;
}

light_bulb& light_bulb::operator =(light_bulb&& a)
{
#ifdef BT_TEST
    std::cout << "operator=(&&)" << std::endl;
#endif
    m_status = std::move(a.m_status);
    m_name = std::move(a.m_name);
    m_ID = std::move(a.m_ID);
    return *this;
}

void light_bulb::on(std::function<void(std::string s)>onOn)
{
    if(m_lock != STATE::UNLOCK)
        return;
    std::lock_guard<std::mutex> lock (m_operationMutex);
    std::stringstream ss;
    ss << ":on:" << m_ID << ";";
    onOn(ss.str());
    m_status = STATE::ON;
}

void light_bulb::off(std::function<void(std::string s)> onOff)
{
    if(m_lock != STATE::UNLOCK)
        return;
    std::lock_guard<std::mutex> lock (m_operationMutex);
    std::stringstream ss;
    ss << ":off:" << m_ID << ";";
    onOff(ss.str());
    m_status = STATE::OFF;
}

STATE light_bulb::getStatus()
{
    std::lock_guard<std::mutex> lock (m_operationMutex);
    return m_status;
}

void light_bulb::setStatus(STATE s)
{
    std::lock_guard<std::mutex> lock (m_operationMutex);
    m_status = s;
}

std::string light_bulb::getName() const
{
    return m_name;
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

std::string light_bulb::dump() const
{
    std::stringstream str;

    str << "bulb name: " << m_name << std::endl;
    str << "bulb ID: " << m_ID << std::endl;
    str << "bulb status: " << stateToString(m_status) << std::endl;

    return str.str();
}
