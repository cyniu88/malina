#include "thermometer_container.h"
#include "../iDom_server_OOP.h"

THERMOMETER *THERMOMETER_CONTAINER::returnThermometerPtr(const std::string& name)
{
    auto m = thermoMap.find(name);
    if (m != thermoMap.end())
        return &(m->second);
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL <<" THERMOMETER_CONTAINER::returnThermometerPtr()"<<std::endl;
        log_file_mutex.mutex_unlock();
      throw std::string("thermometer not found!");
    }
}

THERMOMETER_CONTAINER::THERMOMETER_CONTAINER()
{
}

THERMOMETER::THERMOMETER(int iter):m_stats(iter)
{
    m_className.append(typeid (this).name());
    addToMap(m_className,this);
}

THERMOMETER::THERMOMETER(const THERMOMETER &a):m_thermometer(a.m_thermometer),
    m_stats(a.m_stats)
{
    m_className.append(typeid (this).name());
    addToMap(m_className,this);
}

THERMOMETER::~THERMOMETER()
{
    removeFromMap(m_className);
}

std::string THERMOMETER::dump() const
{
    std::stringstream ret;

    ret << "m_thermometer.maxDate: " << this->m_thermometer.maxDate << std::endl;
    ret << "m_thermometer.minDate: " << this->m_thermometer.minDate << std::endl;
    ret << "m_thermometer.newTemp: " << this->m_thermometer.newTemp << std::endl;
    ret << "m_thermometer.oldTemp: " << this->m_thermometer.oldTemp << std::endl;
    ret << "m_thermometer.maxValue: " << this->m_thermometer.maxValue << std::endl;
    ret << "m_thermometer.minValue: " << this->m_thermometer.minValue << std::endl;
    ret << "m_thermometer.lastState: " << static_cast<int>(this->m_thermometer.lastState) << std::endl;
    ret << "m_thermometer.thermometrName: " << this->m_thermometer.thermometrName << std::endl;

    return ret.str();
}

void THERMOMETER_CONTAINER::add(const std::string &name)
{
    auto pair = std::make_pair(name, THERMOMETER(15));
    pair.second.m_thermometer.thermometrName = name;
    thermoMap.insert(pair);
}

void THERMOMETER_CONTAINER::setTemp(const std::string &name, double value)
{
    returnThermometerPtr(name)->m_thermometer.oldTemp = returnThermometerPtr(name)->m_thermometer.newTemp;
    returnThermometerPtr(name)->m_thermometer.newTemp = value;
}

double THERMOMETER_CONTAINER::getTemp(const std::string &name)
{
    return returnThermometerPtr(name)->m_thermometer.newTemp;
}

double THERMOMETER_CONTAINER::getOldTemp(const std::string &name)
{
    return returnThermometerPtr(name)->m_thermometer.oldTemp;
}

TEMPERATURE_STATE THERMOMETER_CONTAINER::getLastState(const std::string &name)
{
    return returnThermometerPtr(name)->m_thermometer.lastState;
}

void THERMOMETER_CONTAINER::setState(const std::string &name, TEMPERATURE_STATE state)
{
    returnThermometerPtr(name)->m_thermometer.lastState = state;
}

void THERMOMETER_CONTAINER::updateAll(std::vector<std::string> *vectorThermo)
{
    std::string in  = vectorThermo->at(0);
    std::string out = vectorThermo->at(1);
    setTemp("inside", std::stod(in));
    setTemp("outside",std::stod(out));
}

void THERMOMETER_CONTAINER::updateStats(const std::string &name)
{
    returnThermometerPtr(name)->m_stats.push_back(returnThermometerPtr(name)->m_thermometer.newTemp);
}

std::string THERMOMETER_CONTAINER::getStatsByName(const std::string &name)
{
    return returnThermometerPtr(name)->m_stats.stats();
}

bool THERMOMETER_CONTAINER::isMoreDiff(const std::string &name, double diff)
{
    return returnThermometerPtr(name)->m_stats.isMoreDiff(diff);
}

std::pair<double, double> THERMOMETER_CONTAINER::getLast2(const std::string &name)
{
    return returnThermometerPtr(name)->m_stats.getLast2();
}

int THERMOMETER_CONTAINER::sizeOf()
{
    return static_cast<int>(thermoMap.size());
}

void THERMOMETER_CONTAINER::showAll()
{
    for(const auto& n : thermoMap)
    {
        std::cout << n.first << " ";
    }
    std::cout << std::endl << "koniec prointowania w " << std::endl;
}
