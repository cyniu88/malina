#include "command_stop.h"

command_stop::command_stop(std::string name):command(name)
{

}

std::string command_stop::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string ret = "dummy";
    if (v[1]=="server")
    {
       // throw "close";
    }
    else
    {
        ret = "stop wath?";
    }
    return ret;
}

std::string command_stop::help()
{
    std::string help = "stop server - close iDom servre";
    return help;
}
