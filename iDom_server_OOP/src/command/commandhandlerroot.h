#pragma once

#include "commandhandler.h"

class commandHandlerRoot : public commandHandler
{
public:
    explicit commandHandlerRoot(thread_context *context);
    virtual ~commandHandlerRoot() = default;
};

