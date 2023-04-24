#include "satel_integra_stub.h"
#include <iostream>

SATEL_INTEGRA_HANDLER_STUB::SATEL_INTEGRA_HANDLER_STUB()
{
    std::cout << "SATEL_INTEGRA_HANDLER_STUB::SATEL_INTEGRA_HANDLER_STUB()" << std::endl;
}

SATEL_INTEGRA_HANDLER_STUB::~SATEL_INTEGRA_HANDLER_STUB()
{
    std::cout << "SATEL_INTEGRA_HANDLER_STUB::~SATEL_INTEGRA_HANDLER_STUB()" << std::endl;
}

void SATEL_INTEGRA_HANDLER_STUB::checkSatel()
{

}

void SATEL_INTEGRA_HANDLER_STUB::checkAlarm(STATE &st)
{

}

void SATEL_INTEGRA_HANDLER_STUB::run()
{

}

SATEL_INTEGRA_INTERFACE *SATEL_INTEGRA_HANDLER_STUB::getSatelPTR()
{
#ifdef BT_TEST
    std::cout << "SATEL_INTEGRA_HANDLER_STUB::getSatelPTR()" << std::endl;
#endif
    return &m_integra32_stub;
}


bool SATEL_INTEGRA_STUB::isAlarmArmed()
{
    return armed;
}

void SATEL_INTEGRA_STUB::armAlarm(unsigned int partitionID)
{
    armed = true;
}

void SATEL_INTEGRA_STUB::disarmAlarm(unsigned int partitionID)
{
    armed = false;
}

void SATEL_INTEGRA_STUB::outputOn(unsigned int id)
{

}

void SATEL_INTEGRA_STUB::outputOff(unsigned int id)
{

}

void SATEL_INTEGRA_STUB::reconnectIntegra()
{

}

SATEL_INTEGRA_STUB::SATEL_INTEGRA_STUB(){
    std::cout << "SATEL_INTEGRA_STUB::SATEL_INTEGRA_STUB()" << std::endl;
}

SATEL_INTEGRA_STUB::~SATEL_INTEGRA_STUB()
{
    std::cout << "SATEL_INTEGRA_STUB::~SATEL_INTEGRA_STUB()" << std::endl;
}

std::string SATEL_INTEGRA_STUB::getIntegraInfo()
{
    return "test";
}
