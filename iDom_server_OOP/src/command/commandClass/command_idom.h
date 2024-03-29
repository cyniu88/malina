#pragma once

#include <string>
#include <vector>

#include "../command.h"

class command_iDom : public command
{
public:
    explicit command_iDom(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

