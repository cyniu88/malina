#ifndef command_program_H
#define command_program_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_program : public command
{
public:
    explicit command_program(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

#endif // command_program_H
