#ifndef SATEL_INTEGRA_H
#define SATEL_INTEGRA_H

#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#include "../idom_api.h"

class SATEL_INTEGRA : public iDom_API
{
    int m_sock;
    int m_i;
    int m_rozmiar;
    struct sockaddr_in m_server;
    unsigned char m_message[2000];
public:
    SATEL_INTEGRA();
    ~ SATEL_INTEGRA();
    void connectIntegra(const std::string& host, const int port);
    void disconnectIntegra();
    std::string getIntegraInfo();
    std::string checkIntegraOut();
    bool isAlarmArmed();
    std::string dump() const override;
private:
    unsigned short calculateCRC(const std::string& msg);
    int sendIntegra(const std::string& msg);
    int recvIntegra();
    const char* satelType(unsigned char t);
};

#endif // SATEL_INTEGRA_H
