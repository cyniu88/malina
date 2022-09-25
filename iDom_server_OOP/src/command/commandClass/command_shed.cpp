#include "command_shed.h"

command_shed::command_shed(const std::string &name):command(name)
{

}

std::string command_shed::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::stringstream str_buf;
    str_buf << "command shed - wrong paramiter";
    if (v.size() > 1){
        if (v[1] == "put")
        {
            str_buf.str("");
            my_data->lusina.shedJson = nlohmann::json::parse(v[2]);
            my_data->lusina.shedBat.push_back(my_data->lusina.shedJson["bateria"].get<float>());
            my_data->lusina.shedHum.push_back(my_data->lusina.shedJson["wilgotność"].get<int>());
            my_data->lusina.shedPres.push_back(my_data->lusina.shedJson["ciśnienie"].get<float>());
            my_data->lusina.shedTemp.push_back(my_data->lusina.shedJson["temperatura"].get<float>());
            str_buf << "DONE";
            nlohmann::json returnJson;
            returnJson["isDay"] = my_data->main_iDomTools->isItDay();
            returnJson["deepSleep"] = my_data->lusina.shedConfJson["deepSleep"].get<bool>();
            returnJson["howLongDeepSleep"] = my_data->lusina.shedConfJson["howLongDeepSleep"].get<int>();
            returnJson["fanON"] = my_data->lusina.shedConfJson["fanON"].get<bool>();
            my_data->mqttHandler->publish("iDom-domek/data", returnJson.dump());
        }
        else if(v[1] == "show")
        {
            str_buf.str("");
            str_buf << my_data->lusina.shedJson.dump(4);
        }
        else if(v[1] == "set" and v[2] == "deepSleep")
        {
            str_buf.str("");
            my_data->lusina.shedConfJson["deepSleep"] = (v[3] == "true" ? true : false);
            my_data->lusina.shedConfJson["howLongDeepSleep"] = std::stoi(v[4]);
            str_buf << "ustawiono deep sleep "  << std::boolalpha << my_data->lusina.shedConfJson["deepSleep"].get<bool>() <<
                       " na " << my_data->lusina.shedConfJson["howLongDeepSleep"].get<int>()
                    << " sekund." << std::endl;
        }
    }
    return str_buf.str();
}

std::string command_shed::help() const
{
    std::stringstream help;
    help << "shed put <json> - send data from json" << std::endl
     << "shed show - print json data" << std::endl
     << "set deepSleep <bool> <time>" << std::endl;
   return help.str();
}
