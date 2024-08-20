#pragma once

#include "../command.h"

class command_big : public command
{
public:
    explicit command_big(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_context * context) override;
    std::string help() const override;
};
