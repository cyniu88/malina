#ifndef SATEL_INTEGRA_HANDLER_H
#define SATEL_INTEGRA_HANDLER_H

#include "../iDom_server_OOP.h"
#include "satel_integra.h"

class SATEL_INTEGRA_HANDLER
{
    thread_data *my_data;
    SATEL_INTEGRA m_integra32;
public:
    SATEL_INTEGRA_HANDLER() = default;
    explicit SATEL_INTEGRA_HANDLER(thread_data* myData);
    ~SATEL_INTEGRA_HANDLER();

    void checkSatel();
    void checkAlarm(STATE &st);
    void run();

    virtual SATEL_INTEGRA* getSatelPTR();
};

#endif // SATEL_INTEGRA_HANDLER_H
