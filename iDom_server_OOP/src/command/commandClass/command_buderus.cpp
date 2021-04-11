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
        return "done; counter " + std::to_string(++counter);
    }

    else if(v.at(1) == "tapwater_active")
    {
        my_data->ptr_buderus->setTapWater(v.at(2) == "1");
        return "done; counter " + std::to_string(++counter);
    }

    else if(v.at(1) == "boiler_data")
    {
        try{
            my_data->ptr_buderus->updateBoilerDataFromMQTT(nlohmann::json(nlohmann::json::parse(v.at(2))));
        }
        catch(...)
        {
            my_data->iDomAlarm.raiseAlarm(879,"buderus boile_data - wrong JSON format!");
        }

        return "done; counter " + std::to_string(++counter);
    }

    else if(v.at(1) == "thermostat_data")
    {
        try{
        my_data->ptr_buderus->updateThermostatDataFromMQTT(nlohmann::json(nlohmann::json::parse(v.at(2))));
        }
        catch(...){
            my_data->iDomAlarm.raiseAlarm(878,"buderus thermostat_data - wrong JSON format!");
        }

        return "done; counter " + std::to_string(++counter);
    }

    else if(v.at(1) == "print")
    {
        str_buf = my_data->ptr_buderus->getAllData();
    }

    else if(v.at(1) == "circPomp")
    {
         my_data->ptr_buderus->runCirclePompForWhile();
         str_buf = "RUN circle pomp";
    }
    else if(v.at(1) == "boiler" && v.at(2) == "heating")
    {
        my_data->ptr_buderus->boilerHeatOneTime();
        str_buf = "Start heating boiler";
    }
    else if(v.at(1) == "set" && v.at(2) == "temp")
    {
        try {
            (void)std::stof(v.at(3));
        } catch (...) {
            return "cannot convert to float";
        }
        my_data->ptr_buderus->setTempInsideBuilding(v.at(3));
        str_buf = "temp has been set to: " + v.at(3);
    }
    return str_buf;
}

std::string command_buderus::help() const
{
    std::stringstream ret;
    ret << "buderus print - show all data" << std::endl;
    ret << "buderus thermostat_data <json>  - set termostat data" << std::endl;
    ret << "buderus boiler_data <json>  - set boiler data" << std::endl;
    ret << "buderus tapwater_active <1/0>  - set tapwater activ" << std::endl;
    ret << "buderus heating_active <1/0>  - set heating activ" << std::endl;
    ret << "buderus circPomp  - run circle pomp tap water for while" << std::endl;
    ret << "buderus boiler heating - heat boiler one time" << std::endl;
    ret << "buderus set temp <float> - set thermostat temperature <degrees>" << std::endl;

    return ret.str();
}
