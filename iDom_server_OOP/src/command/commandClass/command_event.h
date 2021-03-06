#ifndef COMMAND_EVENT_H
#define COMMAND_EVENT_H
#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_event : public command
{
public:
    explicit command_event(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_EVENT_H
