#include "command_ok.h"

command_ok::command_ok(const std::string &name):command(name)
{
}

std::string command_ok::execute(std::vector<std::string> &v, thread_data *context)
{
    return "\nEND\n";
}

std::string command_ok::help() const
{
    return "ok - confirmation msg server response: END \n";
}
