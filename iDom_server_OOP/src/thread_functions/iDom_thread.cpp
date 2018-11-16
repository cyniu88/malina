#include "iDom_thread.h"
#include "../functions/functions.h"

std::string iDOM_THREAD::start_thread(const std::string& name,
                                      void(* fn),
                                      thread_data* my_data,
                                      int thread_socket = 1)
{
    int freeSlotID = useful_F::findFreeThreadSlot(my_data->main_THREAD_arr);

    if ( freeSlotID != -1)
    {
        my_data->main_THREAD_arr[freeSlotID].thread        = std::thread(useful_F::kodi, my_data);
        my_data->main_THREAD_arr[freeSlotID].thread_name   = name;
        my_data->main_THREAD_arr[freeSlotID].thread_ID     = my_data->main_THREAD_arr[freeSlotID].thread.get_id();
        my_data->main_THREAD_arr[freeSlotID].thread_socket = thread_socket;
        my_data->main_THREAD_arr[freeSlotID].thread.detach();

        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "watek " << name << " wystartowal  "<< my_data->main_THREAD_arr[freeSlotID].thread_ID << std::endl;
        log_file_mutex.mutex_unlock();

        return "DONE - " + name + " STARTED";
    }
    return "not free space to new thread";
}

void iDOM_THREAD::stop_thread(const std::string& name,
                              thread_data* my_data)
{
    try
    {
        for (int i =0 ; i< iDomConst::MAX_CONNECTION; ++i)
        {
            if (my_data->main_THREAD_arr[i].thread_ID == std::this_thread::get_id())
            {
                //my_data->main_THREAD_arr[i].thread.detach();
                my_data->main_THREAD_arr[i].thread_name ="  -empty-  ";
                my_data->main_THREAD_arr[i].thread_ID =  std::thread::id();
                my_data->main_THREAD_arr[i].thread_socket = 0;
                break;
            }
        }
    }
    catch (std::system_error &e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR<< "zlapano wyjatek w  watku " << name << ": " << e.what()<< std::endl;
        log_file_mutex.mutex_unlock();
    }

    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< "koniec  watku" <<name <<  std::endl;
    log_file_mutex.mutex_unlock();
}
