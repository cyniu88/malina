#ifndef SATEL_INTEGRA_HANDLER_H
#define SATEL_INTEGRA_HANDLER_H

#include "../iDom_server_OOP.h"
#include "satel_integra.h"

class SATEL_INTEGRA_HANDLER
{
    thread_data *my_data;
public:
    SATEL_INTEGRA m_integra32;
    explicit SATEL_INTEGRA_HANDLER(thread_data* myData);
    ~SATEL_INTEGRA_HANDLER();

    void checkSatel();
    void run();

};

#endif // SATEL_INTEGRA_HANDLER_H
