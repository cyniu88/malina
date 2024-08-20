#include "command_put.h"
#include "../../iDomTools/idomtools_interface.h"

command_put::command_put(const std::string &name):command(name)
{
}

std::string command_put::execute(std::vector<std::string> &v, thread_context *context)
{
    std::string str_buf = "command put - wrong paramiter: ";
    if (v.size() > 1){
        if (v[1] == "temperature")
        {
            str_buf.erase();
            context->main_iDomTools->send_data_to_thingSpeak();
            str_buf = "DONE";
        }
        else         if (v[1] == "influx")
        {
            str_buf.erase();
            context->main_iDomTools->send_data_to_influxdb();
            str_buf = "DONE";
        }
    }
    return str_buf;
}

std::string command_put::help() const
{
    std::stringstream help;
    help << "put <parameter> - " << std::endl << std::endl;
    help << "parameter:" << std::endl;
    help << "\ttemperature - put actual temperature from inside and outside and smog on thingspeak\n" << std::endl;
    help << "\tinflux - put actual temperature from inside and outside and smog and other to influxdb\n" << std::endl;
    return help.str();
}
