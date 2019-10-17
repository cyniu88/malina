#include "house_room.h"

house_room::house_room(const std::string &name):
    m_name(name)
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
}

house_room::~house_room()
{
    iDom_API::removeFromMap(m_className);
}

house_room::house_room(house_room&& other)
{
    assert(this != &other);

    m_lightBulbMap = std::move(other.m_lightBulbMap);

    m_name = std::move(other.m_name);
}

house_room &house_room::operator=(house_room&& other)
{
    assert(this != &other);


    m_lightBulbMap = std::move(other.m_lightBulbMap);

    m_name = std::move(other.m_name);
    return *this;
}

void house_room::addBulb(const std::string& name, int id, std::map<int, std::shared_ptr<light_bulb>>* lightbulbMapptr)
{
    m_lightBulbMap[id] = std::make_shared<light_bulb>(name, id);
    lightbulbMapptr->insert( std::make_pair(id, m_lightBulbMap[id]) );
}

std::string house_room::getName() const
{
    return m_name;
}

void house_room::on(int id, std::function<void(std::string s)> func)
{
    if(m_lock != STATE::UNLOCK)
        return;

    m_lightBulbMap.at(id)->on(func);
}

void house_room::off(int id, std::function<void(std::string s)> func)
{
    if(m_lock != STATE::UNLOCK)
        return;

    m_lightBulbMap.at(id)->off(func);
}

void house_room::allOn(std::function<void (std::string)> func)
{
    if(m_lock != STATE::UNLOCK)
        return;

    for (auto& ptr: m_lightBulbMap ){
        ptr.second->on(func);
    }
}

void house_room::allOff(std::function<void (std::string)> func)
{
    if(m_lock != STATE::UNLOCK)
        return;

    for (auto& ptr: m_lightBulbMap ){
        ptr.second->off(func);
    }
}

void house_room::lock()
{
    m_lock = STATE::LOCK;
}

void house_room::unlock()
{
    m_lock = STATE::UNLOCK;
}

STATE house_room::getLockState() const
{
    return m_lock;
}

nlohmann::json house_room::getJsonInfoLightBulb() const
{
    nlohmann::json jj;

    for(auto& a : m_lightBulbMap){
        nlohmann::json tempJJ;
        tempJJ["name"] = a.second->getName();
        tempJJ["state"] = stateToString( a.second->getStatus() );
        tempJJ["isLock"] = stateToString( a.second->getLockState() );
        tempJJ["ID"] = a.second->getID();

        jj[a.second->getName()] = tempJJ;
    }
    return jj;
}

std::string house_room::dump() const
{
    std::stringstream str;

    str << "name: " << m_name << std::endl;
    for(auto& a : m_lightBulbMap){
        str << " m_lighyBulbMap: " << a.first << std::endl;
    }

    return str.str();
}
