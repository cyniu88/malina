#include "satel_integra_stub.h"

SATEL_INTEGRA_HANDLER_STUB::SATEL_INTEGRA_HANDLER_STUB()
{
   std::cout << "SATEL_INTEGRA_HANDLER_STUB::SATEL_INTEGRA_HANDLER_STUB()" << std::endl;
}

SATEL_INTEGRA_HANDLER_STUB::~SATEL_INTEGRA_HANDLER_STUB()
{
std::cout << "SATEL_INTEGRA_HANDLER_STUB::~SATEL_INTEGRA_HANDLER_STUB()" << std::endl;
}

SATEL_INTEGRA *SATEL_INTEGRA_HANDLER_STUB::getSatelPTR()
{
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
