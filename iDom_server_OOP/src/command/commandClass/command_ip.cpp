#include "command_ip.h"

command_ip::command_ip(const std::string &name):command(name)
{
}

std::string command_ip::execute(std::vector<std::string> &v, thread_data *context)
{
    return "iDom server IP: " + context->server_settings->_server.SERVER_IP;
}

std::string command_ip::help() const
{
    return "ip - show server IP address\n";
}
