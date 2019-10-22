#ifndef HOUSE_ROOM_H
#define HOUSE_ROOM_H

#include <map>
#include <memory>

#include "../idom_api.h"
#include "light_bulb.h"
#include "json.hpp"

class house_room: public iDom_API
{
    std::map<int, std::shared_ptr<light_bulb>> m_lightBulbMap;
    std::string m_name;
    STATE m_lock = STATE::UNLOCK;

public:
    explicit house_room(const std::string& name);
    ~house_room();
    house_room(house_room &&other);
    house_room& operator=(house_room&& other);

    void addBulb(const std::string& name, int id, std::map<int, std::shared_ptr<light_bulb> > *lightbulbMapptr);

    std::string getName() const;

    void on(int id, std::function<void(std::string s)> func);
    void off(int id, std::function<void(std::string s)> func);

    void allOn(std::function<void(std::string s)> func);
    void allOff(std::function<void(std::string s)> func);

    void lock();
    void unlock();

    STATE getLockState() const;

    nlohmann::json getJsonInfoLightBulb() const;

    std::string dump() const override;
};

#endif // HOUSE_ROOM_H
