#ifndef COMMAND_433MHZ_H
#define COMMAND_433MHZ_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_433MHz : public command
{
public:
    explicit command_433MHz(const std::string& name);
    ~command_433MHz();
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_433MHZ_H
