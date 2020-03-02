#include "command_sleep.h"
#include "../../functions/functions.h"
#include "../../thread_functions/iDom_thread.h"

command_sleep::command_sleep(const std::string &name):command(name)
{
}

std::string command_sleep::execute(std::vector<std::string> &v, thread_data *my_data)
{   if (v.size() == 1)
    {
        return "sleep set to: "+ std::to_string(my_data->sleeper);
    }
    else if (v.size() == 3 ){
        if (v[1] == "set"){
            int sleep = 0;
            try {
                sleep = std::stoi(v[2]);
            }
            catch (...){
                return "system need intiger > 0 not: " + v[2];
            }

            my_data->sleeper = sleep;

            return iDOM_THREAD::start_thread("Sleep MPD",useful_F::sleeper_mpd,my_data);
        }
        else {
            return "wrong parametr " + v[1];
        }
    }
    return "internal error";
}

std::string command_sleep::help() const
{
    std::stringstream help;
    help << "sleep - show actual time to stop play music" << std::endl;
    help << "sleep set <int> - start sleep for <int> minutes" << std::endl;

    return help.str();
}
