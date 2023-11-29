#pragma once

#include "../command.h"

class command_gateway : public command
{
public:
    explicit command_gateway(const std::string &name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

