#pragma once

#include "commandhandlerroot.h"

class commandHandlerRS232 : public commandHandlerRoot
{
public:
    explicit commandHandlerRS232(thread_data *context);
    virtual ~commandHandlerRS232() = default;
};

