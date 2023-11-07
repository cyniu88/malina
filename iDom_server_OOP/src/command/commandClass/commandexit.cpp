#include "commandexit.h"

commandEXIT::commandEXIT(const std::string &name):command(name)
{
}

std::string commandEXIT::execute(std::vector<std::string> &v, thread_data *context)
{
    return "\nEND.\n";
}

std::string commandEXIT::help() const
{
    return "exit - disconnect from server\n";
}
