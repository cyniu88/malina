#ifndef TASKER_H
#define TASKER_H
#include "../iDom_server_OOP.h"
#include "../command/commandhandlerrs232.h"

class TASKER
{
    std::vector <std::string> v;
public:
    TASKER(thread_data *my_data);
    void runTasker();
private:
    thread_data * my_data;
};

#endif // TASKER_H
