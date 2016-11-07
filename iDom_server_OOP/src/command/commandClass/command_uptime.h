#ifndef COMMAND_UPTIME_H
#define COMMAND_UPTIME_H


#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_UPTIME : public command
{
public:
    command_UPTIME(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_UPTIME_H
