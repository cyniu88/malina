#pragma once

#include <string>
#include <vector>
#include "../command.h"

class command_sleep : public command
{
public:
    explicit command_sleep(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_context * context) override;
    std::string help() const override;
};

