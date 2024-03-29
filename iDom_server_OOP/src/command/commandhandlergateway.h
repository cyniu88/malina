#pragma once

#include "commandhandler.h"

class commandHandlerGATEWAY : public commandHandler
{
public:
    explicit commandHandlerGATEWAY(thread_data *context);
    virtual ~commandHandlerGATEWAY() = default;

    std::string run(std::vector<std::string> &v, thread_data *context) override;
};

