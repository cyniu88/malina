#pragma once


#include <map>
#include <exception>
#include "../../SerialPi/serialpi.h"
#include "../../iDom_server_OOP.h"

class WRONG_FORMAT : public std::exception{

};

struct RFLink_DEV{
    int m_counter = 0;
    std::string msg;
    void counter(){
        ++m_counter;
    }

    std::string read(){
        std::stringstream ret;
        ret << m_counter << " \t" << msg << std::endl;
        return ret.str();
    }
};

class RFLinkHandler : public iDom_API
{
    thread_data *context;
    SerialPi m_serial_RFLink;

public:

    static std::mutex sm_RFLink_MUTEX;
    static std::string sm_RFLink_BUFOR;

    unsigned int m_okTime = 0;
    unsigned int m_pingTime = 0;
    ////// temporarnie //////
    std::unordered_map<std::string, RFLink_DEV> m_rflinkMAP;
    /////////////////////////////
    explicit RFLinkHandler(thread_data *context);
    ~RFLinkHandler();
    bool init();
    // void run();
    void flush();
    void sendCommand(std::string cmd);
    std::string sendCommandAndWaitForReceive(std::string cmd);
    std::string readFromRS232();

    std::string dump() const override;

private:
    std::string internalReadFromRS232();
    //#ifdef BT_TEST
public:
    //#endif
    static std::string getArgumentValueFromRFLinkMSG(const std::string &msg, const std::string &var);
};

