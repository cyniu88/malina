#include "command_clock.h"
#include "../../../src/functions/functions.h"

command_clock::command_clock(const std::string &name):command(name)
{

}

std::string command_clock::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size() > 1){
        return "clock set "+ useful_F::send_to_arduino_clock(my_data,v[1]);
    }
    return "can not set clock";
}

std::string command_clock::help() const
{
    return "clock <number/string> - put number/string (max 4 digits) to 7segment LCD\n";
}
