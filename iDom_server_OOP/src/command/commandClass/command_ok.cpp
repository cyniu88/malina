#include "command_ok.h"

command_ok::command_ok(std::string name):command(name)
{
}

std::string command_ok::execute(std::vector<std::string> &v, thread_data *my_data)
{
    return "\nEND\n";
}

std::string command_ok::help()
{
    return "ok -  confirmation msg  server response: END \n";
}
