#pragma once

#include <list>

#include "../satel_integra_interface.h"
#include "../libs/useful/useful.h"

class SATEL_INTEGRA_STUB : public SATEL_INTEGRA_INTERFACE
{
   bool  armed = false;
public:
    SATEL_INTEGRA_STUB();
   ~ SATEL_INTEGRA_STUB();
    void setIntegraPin(const std::string& pin);
    void connectIntegra(const std::string& host, const int port);
    void disconnectIntegra();
    std::string getIntegraInfo();
    std::string checkIntegraOut();
    bool isAlarmArmed();
    void armAlarm(unsigned int partitionID);
    void disarmAlarm(unsigned int partitionID);
    void outputOn(unsigned int id);
    void outputOff(unsigned int id);
    STATE connectionState();
    void reconnectIntegra();
private:
    void calculateCRC(const unsigned char *pCmd, unsigned int length, unsigned short &result);
    int sendIntegra(const unsigned char *cmd, const unsigned int cmdLength);
    int recvIntegra();
    const char* satelType(unsigned char t);
    void expandForSpecialValue(std::list<unsigned char> &result);
    std::string getFullFrame(const unsigned char* pCmd, const unsigned int cmdLength);

};
class SATEL_INTEGRA_HANDLER_STUB : public SATEL_INTEGRA_HANDLER_INTERFACE
{
    SATEL_INTEGRA_STUB m_integra32_stub;
public:
    SATEL_INTEGRA_HANDLER_STUB();
    ~SATEL_INTEGRA_HANDLER_STUB() ;

    void checkSatel() override;
    void checkAlarm(STATE &st) override;
    void run() override;

    SATEL_INTEGRA_INTERFACE *getSatelPTR() override;
};

