#pragma once

#include "commandhandler.h"

class commandHandlerRoot : public commandHandler
{
public:
    explicit commandHandlerRoot(thread_data *context);
    virtual ~commandHandlerRoot() = default;
};

