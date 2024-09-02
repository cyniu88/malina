#include <thread>
#include <chrono>

#include "command_gateway.h"
#include "../../iDomTools/idomtools_interface.h"

command_gateway::command_gateway(const std::string &name) : command(name)
{
}

std::string command_gateway::execute(std::vector<std::string> &v, thread_context *context)
{
    if (v[1] == "fan")
    {
        context->main_iDomTools->turnOn433MHzSwitch("fan");
        return "fan on";
    }
    if (v[1] == "iDomUnlock")
    {
        context->main_iDomTools->unlockHome();
        return "alarm wyłączony";
    }
    else if (v[1] == "sleep")
    {
        auto sendComand = [context](int sleep, std::vector<std::string> command)
        {
            std::this_thread::sleep_for(std::chrono::minutes(sleep)); // Symulacja opóźnienia
            if (command.size() >= 3)
            {
                // Usuń dwa pierwsze elementy
                command.erase(command.begin(), command.begin() + 3);
            }
            std::vector<std::string> cmd = {std::accumulate(command.begin(), command.end(), std::string(), [](const std::string &a, const std::string &b)
                                                            { return a.empty() ? b : a + " " + b; })};
            context->main_iDomTools->runCommandFromJson(cmd);

            log_file_mutex.mutex_lock();
            log_file_cout << DEBUG << "komenda: " << cmd.at(0) << " odłożona w czasie: " << sleep << "minut" << std::endl;
            log_file_mutex.mutex_unlock();
        };

        // Uruchamianie lambdy asynchronicznie
        std::thread t(sendComand, std::stoi(v[2]), v);
        t.detach();

        return "done";
    }
    return "unknown paramiter - " + help();
}

std::string command_gateway::help() const
{
    std::stringstream ret;
    ret << "gateway fan  - turn on fan" << std::endl;
    ret << "gateway sleep (minutes) <command>  -  run command afte some time in minutes" << std::endl;
    return ret.str();
}
