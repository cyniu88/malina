#pragma once

#include "../command.h"

class command_433MHz : public command
{
public:
    explicit command_433MHz(const std::string& name);
    ~command_433MHz() = default;
    std::string execute(std::vector <std::string> &v,thread_context * context) override;
    std::string help() const override;
};

