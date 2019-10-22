#ifndef COMMAND_IP_H
#define COMMAND_IP_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_ip : public command
{
public:
    explicit command_ip(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_IP_H
