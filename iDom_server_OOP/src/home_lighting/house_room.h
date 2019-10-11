#ifndef HOUSE_ROOM_H
#define HOUSE_ROOM_H

#include "../idom_api.h"

class house_room: public iDom_API
{
public:
    house_room();
    std::string dump() const;
};

#endif // HOUSE_ROOM_H
