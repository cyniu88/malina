#ifndef COMMAND_WLACZ_H
#define COMMAND_WLACZ_H

#include "../command.h"

class command_wlacz: public command
{
public:
    explicit command_wlacz(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};


#endif // COMMAND_WLACZ_H
