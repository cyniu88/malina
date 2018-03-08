#ifndef RFLINKHANDLER_H
#define RFLINKHANDLER_H

//#include <mutex>
#include "../../iDom_server_OOP.h"


class RFLinkHandler
{
    thread_data *my_data;
    SerialPi serial_RFLink;

public:

    static std::mutex sm_RFLink_MUTEX;
    static std::string sm_RFLink_BUFOR;
    RFLinkHandler(thread_data *my_data);

    bool init();
    void run();
    void sendCommand(std::string cmd);
    std::string sendCommandAndWaitForReceive(std::string cmd);
private:
    std::string readFromRS232();
};

#endif // RFLINKHANDLER_H
