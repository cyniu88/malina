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
            str_buf << "DONE";
            nlohmann::json returnJson;
            returnJson["isDay"] = my_data->main_iDomTools->isItDay();
            returnJson["deepSleep"] = my_data->lusina.goToDeepSleep;
            returnJson["howLongDeepSleep"] = my_data->lusina.howLongDeepSleep;
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
            my_data->lusina.goToDeepSleep = v[3] == "true" ? true : false;
            my_data->lusina.howLongDeepSleep = std::stoi(v[4]);
            str_buf << "ustawiono deep sleep "  << std::boolalpha << my_data->lusina.goToDeepSleep << " na " << my_data->lusina.howLongDeepSleep
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
