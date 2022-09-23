#include "command_shed.h"

command_shed::command_shed(const std::string &name):command(name)
{

}

std::string command_shed::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command shed - wrong paramiter";
    if (v.size() > 1){
        if (v[1] == "put")
        {
            str_buf.erase();
            my_data->lusina.shedJson = nlohmann::json::parse(v[2]);
            str_buf = "DONE";
        }
        else if(v[1] == "show")
        {
            str_buf.erase();
            str_buf = my_data->lusina.shedJson.dump(4);
        }
    }
    return str_buf;
}

std::string command_shed::help() const
{
    std::stringstream help;
    help << "shed put <json> - send data from json" << std::endl;
    help << "shed show - print json data" << std::endl;
   return help.str();
}
