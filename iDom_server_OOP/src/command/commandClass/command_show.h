#ifndef COMMAND_SHOW_H
#define COMMAND_SHOW_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_show : public command
{
public:
    explicit command_show(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

#endif // COMMAND_SHOW_H
