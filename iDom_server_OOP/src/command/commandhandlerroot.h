#ifndef COMMANDHANDLERROOT_H
#define COMMANDHANDLERROOT_H
#include "commandhandler.h"

class commandHandlerRoot : public commandHandler
{
public:
    commandHandlerRoot(thread_data *my_data);
    ~commandHandlerRoot();
};

#endif // COMMANDHANDLERROOT_H
