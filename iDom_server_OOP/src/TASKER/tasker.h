#ifndef TASKER_H
#define TASKER_H
#include "../command/commandhandlermqtt.h"

// TODO dodac do iDom API
class TASKER
{
    unsigned int counter = 0;
    CommandHandlerMQTT commandMQTT;
    std::string topic;

public:
    explicit TASKER(thread_data *context);

    int runTasker();

private:
    thread_data *context;
};

#endif // TASKER_H
