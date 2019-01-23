#include "command_cmd.h"
#include "../../../src/functions/functions.h"

command_cmd::command_cmd(const std::string &name):command(name)
{

}

std::string command_cmd::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size() == 1){
        return "fifo file contain: "+ useful_F_libs::read_from_mkfifo(my_data->server_settings->_server.omxplayerFile);
    }
    return "error: unknown parameter: "+v[1];
}

std::string command_cmd::help()
{
    return "cmd - read char prom cmd fifo file for unblock video player\n";
}
