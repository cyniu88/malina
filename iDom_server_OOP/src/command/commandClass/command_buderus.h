#pragma once

#include "../command.h"

class command_buderus : public command
{
public:
    explicit command_buderus(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_context * context) override;
    std::string help() const override;
};
