#ifndef IDOM_THREAD_H
#define IDOM_THREAD_H

#include "../iDom_server_OOP.h"

#include <functional>

class iDOM_THREAD
{
private:
    iDOM_THREAD();
public:
    static std::string start_thread(const std::string &name,
                                    std::function<void(thread_data *, const std::string &)> functionToThread,
                                    thread_data *my_data,
                                    unsigned int thread_socket = 1);

    static void stop_thread(const std::string &name,
                            thread_data* my_data);

    static void waitUntilAllThreadEnd(thread_data* my_data);

    static int findFreeThreadSlot(std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> *array);
};

#endif
