#ifndef IDOM_THREAD_H
#define IDOM_THREAD_H

#include  "../iDom_server_OOP.h"
#include "../logger/logger.hpp"

class iDOM_THREAD
{
    static std::string start_thread(const std::string &name,
                                    void(* fn),
                                    thread_data *my_data,
                                    int thread_socket);
    static void stop_thread(const std::string &name,
                            thread_data* my_data);
};

#endif
