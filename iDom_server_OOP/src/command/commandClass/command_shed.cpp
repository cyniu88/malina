#include "command_shed.h"
#include "../../iDomTools/idomtools_interface.h"

command_shed::command_shed(const std::string &name) : command(name)
{
}

std::string command_shed::execute(std::vector<std::string> &v, thread_context *context)
{
    std::stringstream str_buf;
    str_buf << "command shed - wrong paramiter";
    if (v.size() > 1)
    {
        if (v[1] == "put")
        {
            str_buf.str("");
            try
            {
                context->lusina.shedJson = nlohmann::json::parse(v[2]);
            }
            catch (...)
            {
                log_file_mutex.mutex_lock();
                log_file_cout << CRITICAL << " błąd odebranego json z shed " << v[2] << std::endl;
                log_file_mutex.mutex_unlock();

                return "error 222";
            }
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////
            context->lusina.shedFloor.push_back(context->lusina.shedJson["podłoga"].get<float>());
            context->lusina.shedHum.push_back(context->lusina.shedJson["wilgotność"].get<float>());
            context->lusina.shedPres.push_back(context->lusina.shedJson["ciśnienie"].get<float>());
            context->lusina.shedTemp.push_back(context->lusina.shedJson["temperatura"].get<float>());
            context->lusina.acdc.push_back(context->lusina.shedJson["acdc"].get<float>());
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // auto getData = [&v](thread_context *context, const std::string &key)
            // {
            //     try
            //     {
            //         context->lusina.shedFloor.push_back(context->lusina.shedJson[key].get<float>());
            //     }
            //     catch (const std::exception &e)
            //     {
            //         log_file_mutex.mutex_lock();
            //         log_file_cout << CRITICAL << " błąd odebranego json z shed " << v[2] << " klucz: " << key << std::endl;
            //         log_file_mutex.mutex_unlock();
            //     }
            // };
            // auto keyList = {"temperatura", "ciśnienie", "wilgotność", "podłoga", "acdc"};

            // for (const auto &key : keyList)
            // {
            //     getData(context, key);
            // }
            str_buf << "DONE";
            nlohmann::json returnJson;
            returnJson["isDay"] = context->main_iDomTools->isItDay();
            returnJson["deepSleep"] = context->lusina.shedConfJson["deepSleep"].get<bool>();
            returnJson["howLongDeepSleep"] = context->lusina.shedConfJson["howLongDeepSleep"].get<int>();
            returnJson["fanON"] = context->lusina.shedConfJson["fanON"].get<bool>();
            context->mqttHandler->publish("iDom-domek/data", returnJson.dump());
        }
        else if (v[1] == "show")
        {
            str_buf.str("");
            str_buf << context->lusina.shedJson.dump(4);
        }
        else if (v[1] == "set" and v[2] == "deepSleep")
        {
            str_buf.str("");
            context->lusina.shedConfJson["deepSleep"] = (v[3] == "true" ? true : false);
            context->lusina.shedConfJson["howLongDeepSleep"] = std::stoi(v[4]);
            str_buf << "ustawiono deep sleep " << std::boolalpha << context->lusina.shedConfJson["deepSleep"].get<bool>() << " na " << context->lusina.shedConfJson["howLongDeepSleep"].get<int>()
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
         << "shed set deepSleep <bool> <time> - set time for sleep  if" << std::endl;
    return help.str();
}
