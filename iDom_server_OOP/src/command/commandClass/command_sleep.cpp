#include "command_sleep.h"
#include "../iDom_server_OOP/src/functions/functions.h"

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
            for (int con_counter=0; con_counter< MAX_CONNECTION; ++con_counter)
            {
                if ( my_data->main_THREAD_arr[con_counter].thread_ID==0 || pthread_kill(my_data->main_THREAD_arr[con_counter].thread_ID, 0) == ESRCH )   // jesli pozycja jest wolna (0)  to wstaw tam  jesli jest zjęta wyslij sygnal i sprawdz czy waŧek żyje ///

                {
                    if ( con_counter!=MAX_CONNECTION -1)
                    {
                        // int sleep_t = my_data_logic->sleeper;

                        pthread_create (&my_data->main_THREAD_arr[con_counter].thread_ID, NULL,&sleeper_mpd,  my_data);
                        my_data->main_THREAD_arr[con_counter].thread_name="Sleeper  MPD ";
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO << "watek SLEEPER_MPD wystartowal  "<< my_data->main_THREAD_arr[con_counter].thread_ID << std::endl;
                        log_file_mutex.mutex_unlock();
                        // my_data_logic->sleeper=0;
                        //                        my_data_logic->mainLCD->printString(true,1,0,"SLEEPer START");
                        //                        my_data_logic->mainLCD->set_print_song_state(0);
                        //                        who = '!';
                        pthread_detach( my_data->main_THREAD_arr[con_counter].thread_ID );
                        return "DONE \n sleep has set to: "+v[2];


                    }
                }
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
    std::string help = "sleep - show actual time to stop play music";
    help.append("sleep set <int> - start sleep for <int> minutes");
    help.append("\n");

    return help;
}
