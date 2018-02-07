#include "command_cmd.h"
#include "/home/pi/programowanie/iDom_server_OOP/src/functions/functions.h"

command_cmd::command_cmd(std::string name):command(name)
{

}

std::string command_cmd::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size() == 1){
        return "fifo file contain: "+ useful_F::read_from_mkfifo();
    }
    return "error: unknown parameter: "+v[1];
}

std::string command_cmd::help()
{
    return "cmd - read char prom cmd fifo file for unblock video player\n";
}
