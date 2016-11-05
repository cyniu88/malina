#ifndef COMMANDTEST_H
#define COMMANDTEST_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class commandTEST : public command
{
public:
    commandTEST(std::string name);
    std::string execute(std::vector <std::string> &v , thread_data * my_data);
};

#endif // COMMANDTEST_H
