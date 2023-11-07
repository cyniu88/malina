#ifndef COMMAND_BUDERUS_H
#define COMMAND_BUDERUS_H

#include "../command.h"

class command_buderus : public command
{
public:
    explicit command_buderus(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

#endif // COMMAND_BUDERUS_H
