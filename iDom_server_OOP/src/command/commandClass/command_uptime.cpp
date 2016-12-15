#include "command_uptime.h"
#include "../iDom_server_OOP/src/functions/functions.h"

command_UPTIME::command_UPTIME(std::string name):command(name)
{

}

std::string command_UPTIME::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf;
    time(&my_data->now_time);
    str_buf ="uptime: ";
    str_buf +=  sek_to_uptime(difftime(my_data->now_time,my_data->start) );
    return str_buf;
}

std::string command_UPTIME::help()
{
    std::string help = "uptime - show server uptime";
    help.append("\n");
    return help;
}

