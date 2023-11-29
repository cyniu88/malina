#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_hello : public command
{
public:
    explicit command_hello(const std::string& name);
    ~command_hello() = default;
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

