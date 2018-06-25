#include "command_state.h"

command_state::command_state(std::string name):command(name)
{

}

std::string command_state::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size()<2){
        return "need parameter!\n" + help();
    }
    if (v[1] == "all"){
        return my_data->main_iDomStatus->getAllObjectsStateString();
    }
    if (v[1] != "all"){
        return my_data->main_iDomStatus->getObjectStateString(v[1]);
    }

    return "unknown error";
}

std::string command_state::help()
{
    std::string s = "state all/<name>   - show state";
    return s+"\n";
}
