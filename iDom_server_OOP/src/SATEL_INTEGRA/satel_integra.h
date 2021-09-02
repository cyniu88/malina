#ifndef SATEL_INTEGRA_H
#define SATEL_INTEGRA_H

#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<list>

#include "../idom_api.h"
#include "satel_enum.h"
#include "../libs/useful/useful.h"

class SATEL_INTEGRA : public iDom_API
{
    int m_sock;
    int m_i;
    int m_rozmiar;
    struct sockaddr_in m_server;
    unsigned char m_message[2000];
    std::string m_pin;
    STATE m_connectState = STATE::DISCONNECTED;
    std::string m_host;
    int m_port;
public:
    SATEL_INTEGRA(const std::string &host, int port);
    ~ SATEL_INTEGRA();
    void setIntegraPin(const std::string& pin);
    void connectIntegra(const std::string& host, const int port);
    void disconnectIntegra();
    std::string getIntegraInfo();
    std::string checkIntegraOut();
    bool isAlarmArmed();
    void armAlarm();
    void disarmAlarm();
    void outputOn(unsigned int id);
    void outputOff(unsigned int id);
    std::string dump() const override;
    STATE connectionState();
private:
    void calculateCRC(const unsigned char *pCmd, unsigned int length, unsigned short &result);
    int sendIntegra(const unsigned char *cmd, const unsigned int cmdLength);
    int recvIntegra();
    const char* satelType(unsigned char t);
    void expandForSpecialValue(std::list<unsigned char> &result);
    std::pair<unsigned char*, unsigned int> getFullFrame(const unsigned char* pCmd, const unsigned int cmdLength);

};

#endif // SATEL_INTEGRA_H
