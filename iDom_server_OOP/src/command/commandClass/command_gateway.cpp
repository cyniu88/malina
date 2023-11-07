#include "command_gateway.h"
#include "../../iDomTools/idomtools_interface.h"

command_gateway::command_gateway(const std::string &name):command(name)
{

}

std::string command_gateway::execute(std::vector<std::string> &v, thread_data *context)
{
    if(v[1] == "fan")
    {
        context->main_iDomTools->turnOn433MHzSwitch("fan");
        return "fan on";
    }
    if(v[1] == "iDomUnlock")
    {
        context->main_iDomTools->unlockHome();
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
