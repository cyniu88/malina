#ifndef COMMAND_LOG_H
#define COMMAND_LOG_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_log : public command
{
public:
    command_log(std::string);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
private:
   logger_level logLevel(std::string level);
};

#endif // COMMAND_LOG_H
