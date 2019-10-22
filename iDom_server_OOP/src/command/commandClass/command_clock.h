#ifndef COMMAND_CLOCK_H
#define COMMAND_CLOCK_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_clock : public command
{
public:
    explicit command_clock(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_CLOCK_H
