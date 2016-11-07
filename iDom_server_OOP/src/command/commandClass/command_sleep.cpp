#include "command_sleep.h"


command_sleep::command_sleep(std::string name):command(name)
{

}

std::string command_sleep::execute(std::vector<std::string> &v, thread_data *my_data)
{
    return "sleep set to: "+ std::to_string(my_data->sleeper);
}

std::string command_sleep::help()
{
    std::string help = "sleep - show actual time to stop play music";
    help.append("\n");

    return help;
}
