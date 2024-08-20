#pragma once

#include "../iDom_server_OOP.h"

#include <functional>

class iDOM_THREAD
{
private:
    iDOM_THREAD();

public:
    static std::string start_thread(const std::string &name,
                                    std::function<void(thread_context *, const std::string &)> functionToThread,
                                    thread_context *context,
                                    unsigned int thread_socket = 1);

    static void stop_thread(const std::string &name,
                            thread_context *context);

    static void waitUntilAllThreadEnd(thread_context *context);

    static int findFreeThreadSlot(std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> *array);
};

