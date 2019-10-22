#ifndef COMMAND_WIFI_H
#define COMMAND_WIFI_H

#include "../command.h"

class command_wifi: public command
{
public:
    explicit command_wifi(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_WIFI_H
