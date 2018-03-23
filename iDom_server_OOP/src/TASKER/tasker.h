#ifndef TASKER_H
#define TASKER_H
#include "../iDom_server_OOP.h"
#include "../command/commandhandlerrs232.h"

class TASKER
{
    unsigned int counter = 0;
public:
    TASKER(thread_data *my_data);

    void runTasker();
private:
     thread_data * my_data;
};

#endif // TASKER_H
