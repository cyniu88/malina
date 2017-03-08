#ifndef COMMAND_STOP_H
#define COMMAND_STOP_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_stop : public command
{
public:
    command_stop(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_STOP_H
