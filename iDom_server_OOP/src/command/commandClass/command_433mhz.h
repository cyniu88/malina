#ifndef COMMAND_433MHZ_H
#define COMMAND_433MHZ_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_433MHz : public command
{
public:
    command_433MHz(std::string name);
    ~command_433MHz();
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_433MHZ_H
