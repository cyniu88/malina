#include "command_put.h"
#include "/home/pi/programowanie/iDom_server_OOP/src/CRON/cron.hpp"

command_put::command_put(const std::string &name):command(name)
{
}

std::string command_put::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command put - wrong paramiter: ";
    if (v.size() > 1){
        if (v[1] =="temperature")
        {
            str_buf.erase();
            my_data->main_iDomTools->send_temperature_thingSpeak();
            str_buf = "DONE";
        }
    }
    return str_buf;
}

std::string command_put::help()
{
    std::stringstream help;
    help << "put <parameter> - " << std::endl << std::endl;
    help << "parameter:" << std::endl;
    help << "\ttemperature - put actual temperature from inside and outside and smog on thingspeak\n" << std::endl;
    return help.str();
}
