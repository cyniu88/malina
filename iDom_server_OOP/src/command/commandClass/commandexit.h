#ifndef COMMANDEXIT_H
#define COMMANDEXIT_H
#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class commandEXIT : public command
{
public:
    commandEXIT(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);

};

#endif // COMMANDEXIT_H
