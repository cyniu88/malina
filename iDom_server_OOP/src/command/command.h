#pragma once

#include <string>
#include <vector>

#include "../iDom_server_OOP.h"

class command
{
    std::string commandName;

public:
    command(const std::string &commandName);
    virtual ~command() = default;
    virtual std::string execute(std::vector<std::string> &v, thread_context *context) = 0;
    virtual std::string help() const = 0;
    virtual std::string getCommandName() const;
    int counter = 0;
};
