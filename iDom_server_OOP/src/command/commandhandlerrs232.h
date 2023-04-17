#ifndef COMMANDHANDLERRS232_H
#define COMMANDHANDLERRS232_H

#include "commandhandlerroot.h"

class commandHandlerRS232 : public commandHandlerRoot
{
public:
    explicit commandHandlerRS232(thread_data *my_data);
    virtual ~commandHandlerRS232() = default;
};

#endif // COMANDHANDLERRS232_H
