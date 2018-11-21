#include "iDom_thread.h"
#include "../functions/functions.h"
#include <tuple>
#include <array>
#include <functional>

std::string iDOM_THREAD::start_thread(const std::string& name,
                                      std::function<void(thread_data*,const std::string& threadName)> functionToThread, //void(fn)(thread_data),
                                      thread_data* my_data,
                                      int thread_socket)
{
    int freeSlotID = useful_F::findFreeThreadSlot(my_data->main_THREAD_arr);

    if ( freeSlotID != -1)
    {
        my_data->main_THREAD_arr->at(freeSlotID).thread  = std::thread(functionToThread ,my_data, name);

        my_data->main_THREAD_arr->at(freeSlotID).thread_name   = name;
        my_data->main_THREAD_arr->at(freeSlotID).thread_ID     = my_data->main_THREAD_arr->at(freeSlotID).thread.get_id();
        my_data->main_THREAD_arr->at(freeSlotID).thread_socket = thread_socket;
        my_data->main_THREAD_arr->at(freeSlotID).thread.detach();

        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "watek " << name << " wystartowal  "<< my_data->main_THREAD_arr->at(freeSlotID).thread_ID << std::endl;
        log_file_mutex.mutex_unlock();

        return "DONE - " + name + " STARTED";
    }
    return "not free space to new thread";
}

std::string iDOM_THREAD::start_thread_RS232(const std::string &name,
                                            std::function<void (thread_data_rs232 *, const std::string &)> functionToThread,
                                            thread_data* my_data,
                                            thread_data_rs232 *my_data_rs232,
                                            int thread_socket)
{
    int freeSlotID = useful_F::findFreeThreadSlot(my_data->main_THREAD_arr);

    if ( freeSlotID != -1)
    {
        my_data->main_THREAD_arr->at(freeSlotID).thread  = std::thread(functionToThread ,my_data_rs232, name);

        my_data->main_THREAD_arr->at(freeSlotID).thread_name   = name;
        my_data->main_THREAD_arr->at(freeSlotID).thread_ID     = my_data->main_THREAD_arr->at(freeSlotID).thread.get_id();
        my_data->main_THREAD_arr->at(freeSlotID).thread_socket = thread_socket;
        my_data->main_THREAD_arr->at(freeSlotID).thread.detach();

        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "watek " << name << " wystartowal  "<< my_data->main_THREAD_arr->at(freeSlotID).thread_ID << std::endl;
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
            if (my_data->main_THREAD_arr->at(i).thread_ID == std::this_thread::get_id())
            {
                //my_data->main_THREAD_arr[i].thread.detach();
                my_data->main_THREAD_arr->at(i).thread_name ="  -empty-  ";
                my_data->main_THREAD_arr->at(i).thread_ID =  std::thread::id();
                my_data->main_THREAD_arr->at(i).thread_socket = 0;
                break;
            }
        }
    }
    catch (std::system_error &e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR<< "zlapano wyjatek w  watku: " << name << ": " << e.what()<< std::endl;
        log_file_mutex.mutex_unlock();
    }

    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< "koniec  watku: " <<name <<  std::endl;
    log_file_mutex.mutex_unlock();
}

void iDOM_THREAD::waitUntilAllThreadEnd(thread_data *my_data)
{
    int threadCounter = 0;
    int counter = 20;
    do{
        if (--counter == 0){
            puts("niedoczekalem sie konca watkow");
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        threadCounter = 0;
        for(auto i = my_data->main_THREAD_arr->begin(); i < my_data->main_THREAD_arr->end(); ++i)
        {
            threadCounter += i->thread_socket;
            if (i->thread_socket != 0)
                std::cout << "thread name: "<< i->thread_name << std::endl;
        }
        std::cout << "watki pracuja "<<threadCounter<< std::endl;
    } while(threadCounter != 0);
}
