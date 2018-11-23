#include "command_uptime.h"
#include "../../src/functions/functions.h"

command_UPTIME::command_UPTIME(const std::string &name):command(name)
{

}

std::string command_UPTIME::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf;
    time(&my_data->now_time);
    str_buf ="uptime: ";
    str_buf.append(useful_F::sek_to_uptime(difftime(my_data->now_time,my_data->start) ) );
    return str_buf;
}

std::string command_UPTIME::help()
{
    std::stringstream help;
    help << "uptime - show server uptime" << std::endl;
    return help.str();
}
