#ifndef command_program_H
#define command_program_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_program : public command
{
public:
    command_program(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // command_program_H
