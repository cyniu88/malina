#ifndef COMMAND_WYLACZ_H
#define COMMAND_WYLACZ_H

#include "../command.h"

class command_wylacz: public command
{
public:
    explicit command_wylacz(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_WYLACZ_H
