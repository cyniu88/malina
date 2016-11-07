#ifndef COMMAND_SHOW_H
#define COMMAND_SHOW_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_show : public command
{
public:
    command_show(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_SHOW_H
