#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_log : public command
{
public:
    explicit command_log(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
private:
   logger_level logLevel(const std::string& level);
};

