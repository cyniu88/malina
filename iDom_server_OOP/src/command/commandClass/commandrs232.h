#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class commandRS232 : public command
{
public:
    explicit commandRS232(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

