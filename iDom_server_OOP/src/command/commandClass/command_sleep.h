#ifndef COMMAND_SLEEP_H
#define COMMAND_SLEEP_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_sleep : public command
{
public:
    explicit command_sleep(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_SLEEP_H
