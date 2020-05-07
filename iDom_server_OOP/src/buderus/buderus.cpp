#include "buderus.h"

BUDERUS::BUDERUS()
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
#ifdef BT_TEST
    std::cout << "BUDERUS::BUDERUS()" << std::endl;
#endif
}

BUDERUS::~BUDERUS()
{
    iDom_API::removeFromMap(m_className);
}

void BUDERUS::updateBoilerDataFromMQTT(nlohmann::json jj)
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    m_boiler_data = jj;
    m_outdoorTemp = jj.at("outdoorTemp").get<double>();
    m_boilerTemp  = jj.at("wwStorageTemp2").get<double>();
}

void BUDERUS::updateThermostatDataFromMQTT(nlohmann::json jj)
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    m_thermostat_data = jj;
    m_insideTemp = jj["hc1"].at("currtemp").get<double>();
}

void BUDERUS::tapWaterActivated()
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    m_tapwater_active = true;

}

void BUDERUS::tapWaterDeactivated()
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    m_tapwater_active = false;
}

void BUDERUS::heatingActivated()
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    m_heating_active = true;
}

void BUDERUS::heatingDeactivated()
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
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
    std::lock_guard <std::mutex> lock(m_lockGuard);
    return m_heating_active;
}

bool BUDERUS::isTapWaterActiv()
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    return m_tapwater_active;
}

std::string BUDERUS::getAllData()
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    std::stringstream ret;
    ret << dump();

    return ret.str();
}

double BUDERUS::getOutdoorTemp()
{
    return m_outdoorTemp;
}

double BUDERUS::getInsideTemp()
{
    return  m_insideTemp;
}

double BUDERUS::getBoilerTemp()
{
    return m_boilerTemp;
}

std::string BUDERUS::dump() const
{
    std::stringstream ret;

    ret << "{\n\"m_boiler_data\": " << m_boiler_data.dump(4) << "," << std::endl;
    ret << "\"m_thermostat_data\": " << m_thermostat_data.dump(4) << "," << std::endl;
    ret << "\"m_tapwater_active\": " <<  m_tapwater_active << "," << std::endl;
    ret << "\"m_heating_active\": " << m_heating_active << "," << std::endl;

    ret << "\"m_boilerTemp\": " << m_boilerTemp << "," << std::endl;
    ret << "\"m_insideTemp\": " << m_insideTemp << "," << std::endl;
    ret << "\"m_outdoorTemp\": " << m_outdoorTemp << "\n}" << std::endl;

    return ret.str();
}
