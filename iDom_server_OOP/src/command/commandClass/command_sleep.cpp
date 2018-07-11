#include "command_sleep.h"
#include "/home/pi/programowanie/iDom_server_OOP/src/functions/functions.h"

command_sleep::command_sleep(std::string name):command(name)
{
}

std::string command_sleep::execute(std::vector<std::string> &v, thread_data *my_data)
{   if (v.size()== 1)
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
                return "system need intiger > 0 not: " +v[2];
            }

            my_data->sleeper = sleep;

            int freeSlotID = useful_F::findFreeThreadSlot(my_data->main_THREAD_arr);

            if ( freeSlotID != -1)
            {
                my_data->main_THREAD_arr[freeSlotID].thread        = std::thread(useful_F::sleeper_mpd,my_data);
                my_data->main_THREAD_arr[freeSlotID].thread_name   ="Sleeper  MPD ";
                my_data->main_THREAD_arr[freeSlotID].thread_ID     = my_data->main_THREAD_arr[freeSlotID].thread.get_id();
                my_data->main_THREAD_arr[freeSlotID].thread_socket = 1;
                my_data->main_THREAD_arr[freeSlotID].thread.detach();
                log_file_mutex.mutex_lock();
                log_file_cout << INFO << "watek SLEEPER_MPD wystartowal  "<< my_data->main_THREAD_arr[freeSlotID].thread_ID << std::endl;
                log_file_mutex.mutex_unlock();

                return "DONE \n sleep has set to: "+v[2];
            }
            return "not free space to new thread";
        }
        else {
            return "wrong parametr "+v[1];
        }
    }
    return "internal error";
}

std::string command_sleep::help()
{
    std::stringstream help;
    help << "sleep - show actual time to stop play music" << std::endl;
    help << "sleep set <int> - start sleep for <int> minutes" << std::endl;

    return help.str();
}
