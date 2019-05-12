#ifndef COMMAND_HELP_H
#define COMMAND_HELP_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_help : public command
{
public:
    command_help(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help() const;
};

#endif // COMMAND_HELP_H
