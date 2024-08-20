#include "command_big.h"

command_big::command_big(const std::string &name):command(name)
{

}

std::string command_big::execute(std::vector<std::string> &v, thread_context *context)
{
    std::string str_buf = "command big - wrong paramiter:\n "+ help();
    if (v.size() > 1){
        str_buf.erase();
        for (int i = 0; i < std::stoi(v[1])-1; ++i){
            str_buf.push_back('z');
        }
        str_buf.push_back('Y');
    }
    return str_buf;
}

std::string command_big::help() const
{
    return "big <number> - send big (number) data \n";
}
