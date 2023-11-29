#pragma once

#include <string>

#include "../iDom_server_OOP.h"
#include "../command/command.h"

class CommandHandlerMQTT
{
    std::map<std::string, std::unique_ptr<command>> commandMap;

public:
    CommandHandlerMQTT();
    ~CommandHandlerMQTT() = default;
    std::string run(std::vector<std::string> &v, thread_data *context);
};

