#ifndef COMMAND_STATE_H
#define COMMAND_STATE_H
#include "../command.h"

class command_state : public command
{
public:
    command_state(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_STATE_H
