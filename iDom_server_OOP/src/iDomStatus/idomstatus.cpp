#include "idomstatus.h"

iDomSTATUS::iDomSTATUS()
{

}

void iDomSTATUS::addObject(std::string name, STATE st)
{
    std::lock_guard < std::mutex > lock ( m_lockGuard);
    m_stateMAP.insert(std::make_pair(name,st));
}

void iDomSTATUS::setObjectState(std::string name, STATE st)
{
    std::lock_guard < std::mutex > lock ( m_lockGuard);
    auto i = m_stateMAP.find(name);
    if (i != m_stateMAP.end()){
        i->second = st;
    }
    else
    {
        lock.~lock_guard();   //TODO is it correct ?
        addObject(name,st);
    }
}

STATE iDomSTATUS::getObjectState(std::string name)
{
    std::lock_guard < std::mutex > lock ( m_lockGuard);
    auto i = m_stateMAP.find(name);
    if (i != m_stateMAP.end()){
        return i->second ;
    }

    return STATE::ERROR;
}

std::string iDomSTATUS::getObjectStateString(std::string name)
{
    std::lock_guard < std::mutex > lock ( m_lockGuard);
    auto i = m_stateMAP.find(name);
    if (i != m_stateMAP.end()){
        return stateToString( i->second) ;
    }
    return stateToString(STATE::ERROR) + " "+name;
}

std::string iDomSTATUS::getAllObjectsStateString()
{
    std::string st="state: ";
    std::lock_guard < std::mutex > lock( m_lockGuard);
    for (auto elm : m_stateMAP){
        st.append(elm.first);
        st.append("=");
        st.append(stateToString(elm.second));
        st.append(" ");
    }
    return st;
}
