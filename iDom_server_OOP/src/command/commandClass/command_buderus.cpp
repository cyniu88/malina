#include "command_buderus.h"

command_buderus::command_buderus(const std::string &name):command(name)
{

}

std::string command_buderus::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command buderus - wrong paramiter:\n "+ help();

    if(v.size() < 2)
        return str_buf;
    if(v.at(1) == "heating_active")
    {
        my_data->ptr_buderus->setHeating(v.at(2) == "1");
        return "done";
    }

    else if(v.at(1) == "tapwater_active")
    {
        my_data->ptr_buderus->setTapWater(v.at(2) == "1");
        return "done";
    }

    else if(v.at(1) == "boiler_data")
    {
        my_data->ptr_buderus->updateBoilerDataFromMQTT(nlohmann::json(nlohmann::json::parse(v.at(2))));
        return "done";
    }

    else if(v.at(1) == "thermostat_data")
    {
        my_data->ptr_buderus->updateThermostatDataFromMQTT(nlohmann::json(nlohmann::json::parse(v.at(2))));
        return "done";
    }

    else if(v.at(1) == "print")
    {
       str_buf = my_data->ptr_buderus->getAllData();
    }

    return str_buf;
}

std::string command_buderus::help() const
{
    std::stringstream ret;
    ret << "print - show all data" << std::endl;
    ret << "thermostat_data <json>  - set termostat data" << std::endl;
    ret << "boiler_data <json>  - set boiler data" << std::endl;
    ret << "tapwater_active <1/0>  - set tapwater activ" << std::endl;
    ret << "heating_active <1/0>  - set heating activ" << std::endl;

    return ret.str();
}
