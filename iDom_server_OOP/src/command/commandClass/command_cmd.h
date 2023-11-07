#ifndef COMMAND_CMD_H
#define COMMAND_CMD_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_cmd : public command
{
public:
    explicit command_cmd(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

#endif // COMMAND_CMD_H
