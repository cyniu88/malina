#ifndef HOUSE_ROOM_H
#define HOUSE_ROOM_H

#include <map>
#include <memory>

#include "../idom_api.h"
#include "light_bulb.h"

class house_room: public iDom_API
{
    std::map<int, std::unique_ptr<light_bulb>> m_lightBulbMap;
    std::string m_name;
public:
    house_room(std::string& name);
    house_room(const house_room&& other);
    house_room& operator=(const house_room&& other);
    void addBulb(std::string name, int id);
    void on(int id, std::function<void(std::string s)> func);
    void off(int id, std::function<void(std::string s)> func);
    std::string dump() const;
};

#endif // HOUSE_ROOM_H
