#include "command_put.h"
#include "../iDom_server_OOP/src/CRON/cron.hpp"

command_put::command_put(std::string name):command(name)
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
    std::string help = "put <parameter> - ";
                    help.append("\n\n");
                    help.append("parameter:");
                    help.append("\n");
                    help.append("\ttemperature - put actual temperature from inside and outside and smog on thingspeak\n");
                    help.append("\n");
        return help;
}
