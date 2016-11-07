#ifndef COMMAND_PUT_H
#define COMMAND_PUT_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_put : public command
{
public:
    command_put(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_PUT_H
