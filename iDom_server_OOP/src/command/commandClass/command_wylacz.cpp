#include "command_wylacz.h"

command_wylacz::command_wylacz(const std::string &name):command(name)
{

}

std::string command_wylacz::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v[1] == "radio"){
        my_data->main_iDomTools->MPD_stop();
        return "done";
    }
    return "TODO";
}

std::string command_wylacz::help() const
{
     return "TODO";
}
