#ifndef COMMAND_PUT_H
#define COMMAND_PUT_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_put : public command
{
public:
    explicit command_put(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_PUT_H
