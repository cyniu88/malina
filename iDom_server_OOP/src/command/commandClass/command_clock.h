#ifndef COMMAND_CLOCK_H
#define COMMAND_CLOCK_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_clock : public command
{
public:
    command_clock(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help() const;
};

#endif // COMMAND_CLOCK_H
