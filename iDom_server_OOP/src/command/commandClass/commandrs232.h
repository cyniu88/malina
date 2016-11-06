#ifndef COMMANDRS232_H
#define COMMANDRS232_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class commandRS232 : public command
{
    int  counter =0;
public:
    commandRS232(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);

};

#endif // COMMANDRS232_H
