#include "command_state.h"

command_state::command_state(const std::string &name):command(name)
{
}

std::string command_state::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string ret = "need parameter!\n" + help();
    if (v.size() > 1)
    {

        if (v[1] == "all")
        {
            return my_data->main_iDomStatus->getAllObjectsStateString();
        }
        if (v[1] != "all")
        {
            return my_data->main_iDomStatus->getObjectStateString(v[1]);
        }

    }
    return ret;
}

std::string command_state::help()
{
    std::stringstream help;
    help << "state all/<name> - show state" << std::endl;
    return help.str();
}
