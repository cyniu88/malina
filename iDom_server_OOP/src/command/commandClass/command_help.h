#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_help : public command
{
    std::map <std::string, std::unique_ptr<command> >* commandMapPtr;
public:
    command_help(const std::string& name, std::map<std::string, std::unique_ptr<command> > *commandMapPtr);
    std::string execute(std::vector <std::string> &v,thread_context * context) override;
    std::string help() const override;
};

