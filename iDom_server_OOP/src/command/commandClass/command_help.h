#ifndef COMMAND_HELP_H
#define COMMAND_HELP_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_help : public command
{
    std::map <std::string, std::unique_ptr<command> >* commandMapPtr;
public:
    command_help(const std::string& name, std::map<std::string, std::unique_ptr<command> > *commandMapPtr);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_HELP_H
