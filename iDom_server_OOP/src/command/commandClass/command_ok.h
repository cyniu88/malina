#ifndef COMMAND_OK_H
#define COMMAND_OK_H

#include <string>
#include <vector>
#include "../command.h"

class command_ok : public command
{
public:
    explicit command_ok(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_OK_H
