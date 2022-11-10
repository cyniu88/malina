#include "command_gateway.h"

command_gateway::command_gateway(const std::string &name):command(name)
{

}

std::string command_gateway::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if(v[1] == "fan")
    {
        my_data->main_iDomTools->turnOn433MHzSwitch("fan");
        return "fan on";
    }
    if(v[1] == "iDomUnlock")
    {
        my_data->main_iDomTools->unlockHome();
        return "alarm wyłączony";
    }
    return "unknown paramiter";
}

std::string command_gateway::help() const
{
    std::stringstream ret;
    ret << "gateway fan  - turn on fan" << std::endl;
    return ret.str();
}
