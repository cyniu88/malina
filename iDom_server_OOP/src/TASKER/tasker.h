#ifndef TASKER_H
#define TASKER_H
#include "../iDom_server_OOP.h"
#include "../command/commandhandlermqtt.h"

//TODO dodac do iDom API
class TASKER
{
    unsigned int counter = 0;
    CommandHandlerMQTT commandMQTT;
    std::string topic;
public:
    explicit TASKER(thread_data *my_data);

    int runTasker();
private:
     thread_data * my_data;
};

#endif // TASKER_H
