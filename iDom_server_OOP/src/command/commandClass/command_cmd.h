#ifndef COMMAND_CMD_H
#define COMMAND_CMD_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_cmd : public command
{
public:
    command_cmd(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_CMD_H
