#include <unistd.h>

#include "rflinkhandler.h"
#include "../../functions/functions.h"

std::mutex RFLinkHandler::sm_RFLink_MUTEX;
std::string RFLinkHandler::sm_RFLink_BUFOR;


RFLinkHandler::RFLinkHandler(thread_data *my_data):
    m_serial_RFLink(my_data->server_settings->_rflink.RFLinkPort)
{
    this->my_data = my_data;
#ifdef BT_TEST
    std::cout << "RFLinkHandler::RFLinkHandler()"<<std::endl;
#endif
}

RFLinkHandler::~RFLinkHandler()
{
    std::cout << "RFLinkHandler::~RFLinkHandler()"<<std::endl;
}

bool RFLinkHandler::init()
{
    if( access( my_data->server_settings->_rflink.RFLinkPort.c_str(), F_OK ) != -1 )
    {
        m_serial_RFLink.begin(my_data->server_settings->_rflink.RFLinkBaudRate);
        log_file_mutex.mutex_lock();
        log_file_cout << INFO <<"otwarcie portu RS232 RFLink " << my_data->server_settings->_rflink.RFLinkPort << "  "
                      <<my_data->server_settings->_rflink.RFLinkBaudRate<<std::endl;
        log_file_mutex.mutex_unlock();
        return true;
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR <<"brak portu RS232 RFLink " << my_data->server_settings->_rflink.RFLinkPort<<std::endl;
        log_file_mutex.mutex_unlock();
        return false;
    }
}

void RFLinkHandler::flush()
{
    m_serial_RFLink.flush();
}

void RFLinkHandler::sendCommand(std::string cmd)
{
    std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);
    cmd.append("\n\r"); // add NL & CR
    m_serial_RFLink.print(cmd.c_str());
}

std::string RFLinkHandler::sendCommandAndWaitForReceive(std::string cmd)
{
    std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);
    cmd.append("\n\r"); // add NL & CR
    m_serial_RFLink.print(cmd.c_str());
    return internalReadFromRS232();
}

std::string RFLinkHandler::readFromRS232()
{
    std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);
    return internalReadFromRS232();
}

std::string RFLinkHandler::internalReadFromRS232()
{
    std::string buf;

    if(m_serial_RFLink.available() > 0){
        while (true){

            char b = m_serial_RFLink.read();
            if (b == '\n'){
                break;
            }
            buf.push_back(b);
        }
    }
    return buf;
}

std::string RFLinkHandler::getArgumentValueFromRFLinkMSG(const std::string& msg, const std::string& var)
{
    std::string id;

    int pos = msg.find(var + "=");
    if (pos == -1 ){
//        log_file_mutex.mutex_lock();
//        log_file_cout << CRITICAL <<" throw RFLinkHandler::getArgumentValueFromRFLinkMSG()-1"<<std::endl;
//        log_file_mutex.mutex_unlock();
        throw "argument \""+var+"\" not found";
    }
    if (msg.at(0) != '2' || msg.at(1) != '0'){
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL <<"throw RFLinkHandler::getArgumentValueFromRFLinkMSG()-2"<<std::endl;
        log_file_mutex.mutex_unlock();
        throw WRONG_FORMAT();
    }
#ifdef BT_TEST
    std::cout << "znaleziono " << var <<" na pozycji " << pos <<std::endl;
#endif

    for (unsigned int i = 1+pos+var.size();;++i ){
        char t = msg.at(i);
        if (t == ';'){
            break;
        }
        id.push_back(t);
    }
    return id;

}
