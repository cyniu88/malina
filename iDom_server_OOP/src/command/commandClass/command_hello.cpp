#include "command_hello.h"

command_hello::command_hello(const std::string &name):command(name)
{}

std::string command_hello::execute(std::vector<std::string> &v, thread_data *my_data)
{
    return "\nHI You User!\n";
}

std::string command_hello::help() const
{
    return R"(hello - send test message to server if connection is ok, the server response: "HI!")";
}
