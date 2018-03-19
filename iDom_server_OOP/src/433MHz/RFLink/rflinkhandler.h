#ifndef RFLINKHANDLER_H
#define RFLINKHANDLER_H

//#include <mutex>
#include <map>
#include "../../iDom_server_OOP.h"
#include <exception>

class WRONG_FORMAT : public std::exception{

};

struct RFLink_DEV{
    int m_counter = 0;
    std::string msg;
    void counter(){
        ++m_counter;
    }

    std::string read(){
        return std::to_string(m_counter) + " \t"+ msg;
    }
};

class RFLinkHandler
{
    thread_data *my_data;
    SerialPi serial_RFLink;

public:

    static std::mutex sm_RFLink_MUTEX;
    static std::string sm_RFLink_BUFOR;

    ////// temporarnie //////
    std::map<std::string, RFLink_DEV> rflinkMAP;
    /////////////////////////////
    RFLinkHandler(thread_data *my_data);

    bool init();
    //    void run();
    void flush();
    void sendCommand(std::string cmd);
    std::string sendCommandAndWaitForReceive(std::string cmd);
    std::string readFromRS232();
private:
    std::string internalReadFromRS232();
//#ifdef BT_TEST
public:
//#endif
    static std::string getArgumentValueFromRFLinkMSG(std::string msg, std::string var);
};

#endif // RFLINKHANDLER_H
