#ifndef COMMAND_OK_H
#define COMMAND_OK_H

#include <string>
#include <vector>
#include "../command.h"

class command_ok : public command
{
public:
    command_ok(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_OK_H
