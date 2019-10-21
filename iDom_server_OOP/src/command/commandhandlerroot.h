#ifndef COMMANDHANDLERROOT_H
#define COMMANDHANDLERROOT_H
#include "commandhandler.h"

class commandHandlerRoot : public commandHandler
{
public:
    explicit commandHandlerRoot(thread_data *my_data);
    virtual ~commandHandlerRoot();
};

#endif // COMMANDHANDLERROOT_H
