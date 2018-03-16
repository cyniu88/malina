#ifndef TASKER_H
#define TASKER_H
#include "../iDom_server_OOP.h"
#include "../command/commandhandlerrs232.h"

class TASKER
{
    commandHandlerRS232 rs232;
public:
    TASKER(thread_data *my_data);

    void runTasker();
    void dataFromRS232(std::string bufor);
private:
     thread_data * my_data;
};

#endif // TASKER_H
