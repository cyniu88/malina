#ifndef COMMANDHANDLERMQTT_H
#define COMMANDHANDLERMQTT_H

#include <string>

#include "../iDom_server_OOP.h"
// #include "command.h"
class command;
class CommandHandlerMQTT
{
    std::map<std::string, std::unique_ptr<command>> commandMap;

public:
    CommandHandlerMQTT();
    std::string run(std::vector<std::string> &v, thread_data *my_data);
};

#endif // COMMANDHANDLERMQTT_H
