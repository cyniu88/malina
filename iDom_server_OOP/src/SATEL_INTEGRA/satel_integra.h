#pragma once

#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <list>
#include <mutex>

#include "../idom_api.h"
#include "../libs/useful/useful.h"
#include "satel_integra_interface.h"

class SATEL_INTEGRA : public SATEL_INTEGRA_INTERFACE, public iDom_API
{
    int m_sock;
    int m_i;
    int m_rozmiar;
    struct sockaddr_in m_server;
    std::string m_pin;
    STATE m_connectState = STATE::DISCONNECTED;
    std::string m_host;
    int m_port;
    std::mutex sendMutex;
#ifdef BT_TEST
public:
#endif
    unsigned char m_message[2000];

public:
    SATEL_INTEGRA();
    SATEL_INTEGRA(const std::string &host, int port);
    ~SATEL_INTEGRA();
    void setIntegraPin(const std::string &pin);
    void connectIntegra(const std::string &host, const int port);
    void disconnectIntegra();
    void reconnectIntegra() override;
    std::string getIntegraInfo() override;
    std::string checkIntegraOut();
    bool isAlarmArmed() override;
    void armAlarm(unsigned int partitionID) override;
    void disarmAlarm(unsigned int partitionID) override;
    void outputOn(unsigned int id) override;
    void outputOff(unsigned int id) override;
    std::string dump() const override;
    STATE connectionState();

private:
    void calculateCRC(const unsigned char *pCmd, unsigned int length, unsigned short &result);
    int sendIntegra(const unsigned char *cmd, const unsigned int cmdLength);
    int recvIntegra();
    const char *satelType(unsigned char t);
    void expandForSpecialValue(std::list<unsigned char> &result);
    std::string getFullFrame(const unsigned char *pCmd, const unsigned int cmdLength);
};

