#pragma once

#include "commandhandler.h"

class commandHandlerGATEWAY : public commandHandler
{
public:
    explicit commandHandlerGATEWAY(thread_context *context);
    virtual ~commandHandlerGATEWAY() = default;

    std::string run(std::vector<std::string> &v, thread_context *context) override;
};

