#include "buderus.h"

BUDERUS::BUDERUS()
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
}

BUDERUS::~BUDERUS()
{
    iDom_API::removeFromMap(m_className);
}

void BUDERUS::updateBoilerDataFromMQTT(nlohmann::json jj)
{
    std::lock_guard <std::mutex> lock( m_lockGuard);
    m_boiler_data = jj;
}

void BUDERUS::updateThermostatDataFromMQTT(nlohmann::json jj)
{
    std::lock_guard <std::mutex> lock ( m_lockGuard);
    m_thermostat_data = jj;
}

void BUDERUS::tapWaterActivated()
{
    std::lock_guard <std::mutex> lock ( m_lockGuard);
    m_tapwater_active = true;

}

void BUDERUS::tapWaterDeactivated()
{
    std::lock_guard <std::mutex> lock ( m_lockGuard);
    m_tapwater_active = false;
}

void BUDERUS::heatingActivated()
{
    std::lock_guard <std::mutex> lock ( m_lockGuard);
    m_heating_active = true;
}

void BUDERUS::heatingDeactivated()
{
    std::lock_guard <std::mutex> lock ( m_lockGuard);
    m_heating_active = false;
}

void BUDERUS::setHeating(bool b)
{
    m_heating_active = b;
}

void BUDERUS::setTapWater(bool b)
{
    m_tapwater_active = b;
}

bool BUDERUS::isHeatingActiv()
{
    std::lock_guard <std::mutex> lock ( m_lockGuard);
    return m_heating_active;
}

bool BUDERUS::isTapWaterActiv()
{
    std::lock_guard <std::mutex> lock ( m_lockGuard);
    return m_tapwater_active;
}

std::string BUDERUS::dump() const
{
    std::stringstream ret;

    ret << "m_boiler_data: " << m_boiler_data.dump(4) << std::endl;
    ret << "m_thermostat_data " << m_thermostat_data.dump(4) << std::endl;
    ret << "m_tapwater_active " <<  m_tapwater_active << std::endl;
    ret << "m_heating_active " << m_heating_active << std::endl;

    return ret.str();
}
