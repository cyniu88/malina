#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_light : public command
{
public:
    explicit command_light(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

