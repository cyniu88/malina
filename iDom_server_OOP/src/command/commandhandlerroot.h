#ifndef COMMANDHANDLERROOT_H
#define COMMANDHANDLERROOT_H

#include "commandhandler.h"

class commandHandlerRoot : public commandHandler
{
public:
    explicit commandHandlerRoot(thread_data *context);
    virtual ~commandHandlerRoot() = default;
};

#endif // COMMANDHANDLERROOT_H
