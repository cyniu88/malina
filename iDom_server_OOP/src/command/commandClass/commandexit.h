#ifndef COMMANDEXIT_H
#define COMMANDEXIT_H
#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class commandEXIT : public command
{
public:
    commandEXIT(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMANDEXIT_H
