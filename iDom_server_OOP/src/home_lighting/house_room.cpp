#include "house_room.h"

house_room::house_room(std::string& name):
                                            m_name(name)
{

}

house_room::house_room(const house_room&& other)
{
    *this = std::move(other);
}

house_room &house_room::operator=(const house_room&& other)
{

    for(auto& a : other.m_lightBulbMap){
        m_lightBulbMap[a.first] = std::move(other.m_lightBulbMap.at(a.first));
    }
    m_name = std::move(other.m_name);
    return *this;
}

void house_room::addBulb(std::string name, int id)
{
    m_lightBulbMap[id] = std::move(std::make_unique<light_bulb>(name, id));
}

void house_room::on(int id, std::function<void(std::string s)> func)
{
    m_lightBulbMap.at(id)->on(func);
}

void house_room::off(int id, std::function<void(std::string s)> func)
{
    m_lightBulbMap.at(id)->off(func);
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
