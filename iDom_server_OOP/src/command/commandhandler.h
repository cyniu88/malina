#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../iDom_server_OOP.h"
#include "../command/command.h"

class commandHandler
{
    thread_data *context;

public:
    std::map<std::string, std::unique_ptr<command>> commandMap;
    explicit commandHandler(thread_data *context);
    virtual ~commandHandler() = default;
    virtual std::string run(std::vector<std::string> &v, thread_data *context);
};

