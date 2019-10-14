#include "house_room.h"

house_room::house_room(std::string& name):
                                            m_name(name)
{

}

house_room::house_room(house_room&& other)
{
   // *this = std::move(other);
   m_lightBulbMap = other.m_lightBulbMap;

   m_name = other.m_name;
}

house_room &house_room::operator=(house_room&& other)
{

    m_lightBulbMap = other.m_lightBulbMap;

    m_name = std::move(other.m_name);
    return *this;
}

void house_room::addBulb(std::string name, int id, std::map<int, std::shared_ptr<light_bulb>>* lightbulbMapptr)
{
    m_lightBulbMap[id] = std::make_shared<light_bulb>(name, id);
    lightbulbMapptr->insert( std::make_pair(id, m_lightBulbMap[id])    );
}

void house_room::on(int id, std::function<void(std::string s)> func)
{
    m_lightBulbMap.at(id)->on(func);
}

void house_room::off(int id, std::function<void(std::string s)> func)
{
    m_lightBulbMap.at(id)->off(func);
}

void house_room::allOn(std::function<void (std::string)> func)
{
    for (auto& ptr: m_lightBulbMap ){
        ptr.second->on(func);
    }
}

void house_room::allOff(std::function<void (std::string)> func)
{
    for (auto& ptr: m_lightBulbMap ){
        ptr.second->off(func);
    }
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
