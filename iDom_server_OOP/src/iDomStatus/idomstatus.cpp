#include "idomstatus.h"
#include "../functions/functions.h"

iDomSTATUS::iDomSTATUS()
{
#ifdef BT_TEST
    std::cout << "iDomSTATUS::iDomSTATUS()" << std::endl;
#endif
}

iDomSTATUS::~iDomSTATUS()
{
#ifdef BT_TEST
    std::cout << "iDomSTATUS::~iDomSTATUS()" << std::endl;
#endif
}

void iDomSTATUS::addObject(std::string name, STATE st)
{
    std::lock_guard < std::mutex > lock ( m_lockGuard);
    m_stateMAP.insert(std::make_pair(name,st));
}

void iDomSTATUS::setObjectState(const std::string& name, STATE st)
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    auto i = m_stateMAP.find(name);
    if (i not_eq m_stateMAP.end()){
        i->second = st;
    }
    else
    {
        lock.~lock_guard();
        addObject(name,st);
    }
}

STATE iDomSTATUS::getObjectState(const std::string& name)
{
    std::lock_guard <std::mutex> lock(m_lockGuard);
    auto i = m_stateMAP.find(name);
    if (i not_eq m_stateMAP.end())
    {
        return i->second;
    }
    return STATE::UNKNOWN;
}

std::string iDomSTATUS::getObjectStateString(const std::string& name)
{
    std::stringstream dataStr;
    std::lock_guard < std::mutex > lock ( m_lockGuard);
    auto i = m_stateMAP.find(name);
    if (i not_eq m_stateMAP.end())
    {
        dataStr << i->second;
        return dataStr.str();
    }
    dataStr << STATE::UNKNOWN << " " << name;
    return dataStr.str();
}

std::string iDomSTATUS::getAllObjectsStateString()
{
    std::stringstream st;
    st << "state: ";
    std::lock_guard < std::mutex > lock( m_lockGuard);
    for (const auto& elm : m_stateMAP)
    {
        st << elm.first << "=";
        st << elm.second << " ";
    }
    // shortcut solution for buderus
    st << "burnGas=";
    auto heating = useful_F::myStaticData->ptr_buderus->isHeatingActiv();
    if (heating == true) {
        st << STATE::ACTIVE;
        st << " burnGasStartTime="
           << useful_F::myStaticData->ptr_buderus->getHeatingStartTime() << ' ';
    }
    else
        st << STATE::DEACTIVE << ' ';
    return st.str();
}
