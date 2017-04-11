#include "command_stop.h"
#include "../../functions/functions.h"

command_stop::command_stop(std::string name):command(name)
{

}

std::string command_stop::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string ret = "dummy";
    if (v.size() <2 )
    {
        return "stop what?";
    }
    if (v[1]=="server")
    {
        useful_F::send_to_arduino_clock(my_data, "STOP");
        iDomTOOLS::MPD_stop();
        throw "close server";
    }
    else
    {
        ret = "stop what? - "+ v[1];
    }
    return ret;
}

std::string command_stop::help()
{
    std::string help = "stop server - close iDom servre";
    return help;
}
