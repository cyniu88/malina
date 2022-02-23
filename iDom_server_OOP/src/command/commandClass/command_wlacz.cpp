#include "command_wlacz.h"

command_wlacz::command_wlacz(const std::string &name):command(name)
{
}

std::string command_wlacz::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v[1] == "radio"){
        my_data->main_iDomTools->MPD_play(my_data);
        return "done";
    }
    return "TODO";
}

std::string command_wlacz::help() const
{
    return "TODO";
}
