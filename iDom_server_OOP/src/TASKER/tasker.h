#pragma once

#include "../command/commandhandlermqtt.h"

// TODO dodac do iDom API
class TASKER
{
    unsigned int counter = 0;
    CommandHandlerMQTT commandMQTT;
    std::string topic;

public:
    explicit TASKER(thread_context *context);

    int runTasker();

private:
    thread_context *context;
};

