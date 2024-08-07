#pragma once

#include "../command.h"

class command_shed : public command
{
public:
    command_shed(const std::string &name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};
