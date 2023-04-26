#ifndef COMMANDHANDLE_H
#define COMMANDHANDLE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../iDom_server_OOP.h"
#include "../command/command.h"

class commandHandler
{
    thread_data *my_data;

public:
    std::map<std::string, std::unique_ptr<command>> commandMap;
    explicit commandHandler(thread_data *my_data);
    virtual ~commandHandler() = default;
    virtual std::string run(std::vector<std::string> &v, thread_data *my_data);
};

#endif // COMMANDHANDLE_H
