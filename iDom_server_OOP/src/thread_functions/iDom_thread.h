#ifndef IDOM_THREAD_H
#define IDOM_THREAD_H

#include  "../iDom_server_OOP.h"
#include "../logger/logger.hpp"

#include <functional>

class iDOM_THREAD
{
public:
    static std::string start_thread(const std::string &name,
                                    std::function<void(thread_data *, const std::string &)> functionToThread,
                                    thread_data *my_data,
                                    int thread_socket = 1);

    static std::string start_thread_RS232(const std::string &name,
                                    std::function<void(thread_data_rs232 *, const std::string &)> functionToThread,
                                    thread_data_rs232 *my_data,
                                    int thread_socket = 1);

    static void stop_thread(const std::string &name,
                            thread_data* my_data);
};

#endif
