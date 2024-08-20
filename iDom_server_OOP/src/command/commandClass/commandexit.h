#pragma once

#include "../command.h"

class commandEXIT : public command
{
public:
    explicit commandEXIT(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_context * context) override;
    std::string help() const override;
};

