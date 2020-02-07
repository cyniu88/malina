#include "command_buderus.h"

command_buderus::command_buderus(const std::string &name):command(name)
{

}

std::string command_buderus::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command buderus - wrong paramiter:\n "+ help();

    return str_buf;
}

std::string command_buderus::help() const
{
    std::stringstream ret;
    return ret.str();
}
