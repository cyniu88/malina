#ifndef HOUSE_LIGHTING_HANDLER_H
#define HOUSE_LIGHTING_HANDLER_H

#include "../idom_api.h"

class house_lighting_handler: public iDom_API
{
public:
    house_lighting_handler();
    std::string dump() const;
};

#endif // HOUSE_LIGHTING_HANDLER_H
