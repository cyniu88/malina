#ifndef COMMAND_GATEWAY_H
#define COMMAND_GATEWAY_H
#include "../command.h"

class command_gateway : public command
{
public:
    command_gateway(const std::string &name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help() const;
};

#endif // COMMAND_GATEWAY_H
