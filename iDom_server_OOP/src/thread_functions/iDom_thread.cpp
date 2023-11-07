#include <tuple>
#include <array>
#include <functional>

#include "iDom_thread.h"

using namespace std::chrono_literals;

std::string iDOM_THREAD::start_thread(const std::string &name,
                                      std::function<void(thread_data *, const std::string &threadName)> functionToThread, // void(fn)(thread_data),
                                      thread_data *context,
                                      unsigned int thread_socket)
{
    int freeSlotID = iDOM_THREAD::findFreeThreadSlot(context->main_THREAD_arr);

    if (freeSlotID not_eq -1)
    {
        std::size_t it = static_cast<std::size_t>(freeSlotID);
        context->main_THREAD_arr->at(it).thread = std::thread(functionToThread, context, name);

        context->main_THREAD_arr->at(it).thread_name = name;
        context->main_THREAD_arr->at(it).thread_ID = context->main_THREAD_arr->at(it).thread.get_id();
        context->main_THREAD_arr->at(it).thread_socket = thread_socket;
        context->main_THREAD_arr->at(it).thread.detach();

        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "watek " << name << " wystartowal "
                      << context->main_THREAD_arr->at(it).thread_ID << std::endl;
        log_file_mutex.mutex_unlock();

        return "DONE - " + name + " STARTED";
    }
    return "not free space to new thread";
}

void iDOM_THREAD::stop_thread(const std::string &name,
                              thread_data *context)
{
    try
    {
        for (std::size_t i = 0; i < iDomConst::MAX_CONNECTION; ++i)
        {
            if (context->main_THREAD_arr->at(i).thread_ID == std::this_thread::get_id())
            {
                context->main_THREAD_arr->at(i).thread_name = "  -empty-  ";
                context->main_THREAD_arr->at(i).thread_ID = std::thread::id();
                context->main_THREAD_arr->at(i).thread_socket = 0;
                break;
            }
        }
    }
    catch (std::system_error &e)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "zlapano wyjatek w watku: " << name << ": " << e.what() << std::endl;
        log_file_mutex.mutex_unlock();
    }

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "koniec watku: " << name << std::endl;
    log_file_mutex.mutex_unlock();
}

void iDOM_THREAD::waitUntilAllThreadEnd(thread_data *context)
{
    int threadCounter = 0;
    int counter = 20;
    do
    {
        if (--counter == 0)
        {
            puts("niedoczekalem sie konca watkow");
            break;
        }
        std::this_thread::sleep_for(1.5s);
        threadCounter = 0;
        for (auto i = context->main_THREAD_arr->begin(); i < context->main_THREAD_arr->end(); ++i)
        {
            threadCounter += i->thread_socket;
            if (i->thread_socket not_eq 0)
                std::cout << "thread name: " << i->thread_name << std::endl;
        }
        std::cout << "watki pracuja " << threadCounter << std::endl;
    } while (threadCounter not_eq 0);
}

int iDOM_THREAD::findFreeThreadSlot(std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> *array)
{
    for (std::size_t i = 0; i < array->size(); ++i)
    {
        if (array->at(i).thread_socket == 0)
            return static_cast<int>(i);
    }
    return -1;
}
