#ifndef COMMAND_LOG_H
#define COMMAND_LOG_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_log : public command
{
public:
    command_log(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help() const;
private:
   logger_level logLevel(const std::string& level);
};

#endif // COMMAND_LOG_H
