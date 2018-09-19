#ifndef COMMAND_BIG_H
#define COMMAND_BIG_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_big : public command
{
public:
    command_big(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_BIG_H
