#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_program : public command
{
public:
    explicit command_program(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_context * context) override;
    std::string help() const override;
};

