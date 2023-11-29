#pragma once

#include "../libs/useful/useful.h"

class SATEL_INTEGRA;
class SATEL_INTEGRA_INTERFACE
{

public:
    SATEL_INTEGRA_INTERFACE(){};
    virtual ~SATEL_INTEGRA_INTERFACE(){};

    virtual bool isAlarmArmed() = 0;
    virtual void armAlarm(unsigned int partitionID) = 0;
    virtual void disarmAlarm(unsigned int partitionID) = 0;
    virtual void outputOn(unsigned int id) = 0;
    virtual void outputOff(unsigned int id) = 0;
    virtual std::string getIntegraInfo() = 0;
    virtual void reconnectIntegra() = 0;
};

class SATEL_INTEGRA_HANDLER_INTERFACE
{
public:
    SATEL_INTEGRA_HANDLER_INTERFACE(){};
    virtual ~SATEL_INTEGRA_HANDLER_INTERFACE(){};

    virtual void checkSatel() = 0;
    virtual void checkAlarm(STATE &st) = 0;
    virtual void run() = 0;

    virtual SATEL_INTEGRA_INTERFACE *getSatelPTR() = 0;
};

