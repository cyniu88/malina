#ifndef COMMANDHANDLERGATEWAY_H
#define COMMANDHANDLERGATEWAY_H

#include "commandhandler.h"

class commandHandlerGATEWAY: commandHandler
{
public:
    commandHandlerGATEWAY(thread_data* my_data);
    virtual ~commandHandlerGATEWAY();

    std::string run(std::vector <std::string> &v, thread_data *my_data);
};

#endif // COMMANDHANDLERGATEWAY_H
