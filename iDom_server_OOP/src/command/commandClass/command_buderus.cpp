#include "command_buderus.h"

command_buderus::command_buderus(const std::string &name):command(name)
{

}

std::string command_buderus::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command buderus - wrong paramiter:\n "+ help();

    if(v.at(1) == "heating_active")
    {
        my_data->ptr_buderus->setHeating(v.at(2) == "1");
        return "done";
    }

    else if(v.at(1) == "boiler_data")
    {
        my_data->ptr_buderus->updateBoilerDataFromMQTT(nlohmann::json(nlohmann::json::parse(v.at(2))));
        return "done";
    }
    return str_buf;
}

std::string command_buderus::help() const
{
    std::stringstream ret;
    return ret.str();
}
