#pragma once

#include "../iDom_server_OOP.h"
#include "satel_integra_interface.h"
#include "satel_integra.h"

class SATEL_INTEGRA_HANDLER : public SATEL_INTEGRA_HANDLER_INTERFACE
{
    thread_context *context;
    public:
    SATEL_INTEGRA m_integra32;

public:
    SATEL_INTEGRA_HANDLER() = default;
    explicit SATEL_INTEGRA_HANDLER(thread_context *myData);
    ~SATEL_INTEGRA_HANDLER();

    void checkSatel() override;
    void checkAlarm(STATE &st) override;
    void run() override;

    SATEL_INTEGRA_INTERFACE *getSatelPTR() override;
};

