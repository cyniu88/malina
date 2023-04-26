#ifndef COMMAND_HELLO_H
#define COMMAND_HELLO_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_hello : public command
{
public:
    explicit command_hello(const std::string& name);
    ~command_hello() = default;
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_HELLO_H
