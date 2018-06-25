#include "command_big.h"

command_big::command_big(std::string name):command(name)
{

}

std::string command_big::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command big - wrong paramiter:\n "+ help();
    if (v.size() > 1){
        str_buf.erase();
        for (int i =0 ; i < std::stoi(v[1])-1 ; ++i){
            str_buf += "z";
        }
        str_buf += "Y";
    }
    return str_buf;
}

std::string command_big::help()
{
    return "big <number> - send big (number)  data \n";
}
