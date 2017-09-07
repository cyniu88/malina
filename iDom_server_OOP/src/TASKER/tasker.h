#ifndef TASKER_H
#define TASKER_H
#include "../iDom_server_OOP.h"

class TASKER
{
public:
    TASKER(thread_data *my_data);

    void runTasker();
    void dataFromRS232(std::string bufor);
private:
     thread_data * my_data;
};

#endif // TASKER_H
