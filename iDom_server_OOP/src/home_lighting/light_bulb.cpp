#include "light_bulb.h"

light_bulb::light_bulb(std::string &name, int id): m_name(name), m_ID(id)
{

}

light_bulb::light_bulb(light_bulb &a):
    m_status(a.m_status),
    m_name(a.m_name),
    m_ID(a.m_ID)
{

}

void light_bulb::on(std::function<void(std::string s)>onOn)
{
    std::lock_guard<std::mutex> lock (m_operationMutex);
    std::stringstream ss;
    ss << ":on:" << m_ID << ";";
    onOn(ss.str());
    m_status = STATE::ON;
}

void light_bulb::off(std::function<void(std::string s)> onOff)
{
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

std::string light_bulb::getName()
{
    return m_name;
}

std::string light_bulb::dump() const
{
    std::stringstream str;
    str << "bulb name: " << m_name << std::endl;
    str << "bulb ID: " << m_ID << std::endl;
    str << "bulb status: " << stateToString(m_status) << std::endl;
    return str.str();
}
