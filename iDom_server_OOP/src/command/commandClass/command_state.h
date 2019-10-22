#ifndef COMMAND_STATE_H
#define COMMAND_STATE_H
#include "../command.h"

class command_state : public command
{
public:
    explicit command_state(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_STATE_H
