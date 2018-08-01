#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../../../iDom_server_OOP/src/iDom_server_OOP.h"

struct thread_data;

class command
{
    std::string commandName;

public:
    command(std::string commandName);
    virtual ~command();
    virtual std::string execute(std::vector<std::string> &v, thread_data * my_data) = 0 ;
    virtual std::string help() = 0;
    virtual std::string getCommandName() ;
};

#endif // COMMAND_H
