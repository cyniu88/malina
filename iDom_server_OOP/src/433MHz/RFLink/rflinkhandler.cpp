#include <unistd.h>
#include "rflinkhandler.h"
#include "../../functions/functions.h"

std::mutex RFLinkHandler::sm_RFLink_MUTEX;
std::string RFLinkHandler::sm_RFLink_BUFOR;

RFLinkHandler::RFLinkHandler(thread_data *my_data):
    serial_RFLink(strdup( my_data->server_settings->RFLinkPort.c_str()))
{
    this->my_data = my_data;
#ifdef BT_TEST
    std::cout << "RFLinkHandler::RFLinkHandler()"<<std::endl;
#endif
}

bool RFLinkHandler::init()
{
    if( access( my_data->server_settings->RFLinkPort.c_str(), F_OK ) != -1 )
    {
        serial_RFLink.begin( std::stoi(my_data->server_settings->RFLinkBaudRate));
#ifndef BT_TEST
        log_file_mutex.mutex_lock();
        log_file_cout << INFO <<"otwarcie portu RS232 RFLink " << my_data->server_settings->RFLinkPort << "  "
                      <<my_data->server_settings->RFLinkBaudRate<<std::endl;
        log_file_mutex.mutex_unlock();
#endif
        return true;
    }
    else
    {
#ifndef BT_TEST
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR <<"brak portu RS232 RFLink " << my_data->server_settings->RFLinkPort<<std::endl;
        log_file_mutex.mutex_unlock();
#endif
        return false;
    }
}

void RFLinkHandler::sendCommand(std::string cmd)
{
    std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);
    cmd.append("\n\r"); // add NL & CR
    serial_RFLink.print(cmd.c_str());
}

std::string RFLinkHandler::sendCommandAndWaitForReceive(std::string cmd)
{
    std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);
    cmd.append("\n\r"); // add NL & CR
    serial_RFLink.print(cmd.c_str());
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
    char b;
    if(serial_RFLink.available() > 0){
        //puts("jest cos na rflinku");
        while (true){

            b = serial_RFLink.read();
            if (b == '\n'){
                break;
            }
            buf += b;
        }
    }
    return buf;
}

std::string RFLinkHandler::getArgumentValueFromRFLinkMSG(std::string msg, std::string var)
{
    std::string id;
    char t;

    int pos = msg.find(var+"=");
    if (pos == -1 ){
        throw "argument \""+var+"\" not found";
    }
    if (msg.at(0) != '2' || msg.at(1) != '0'){
        throw "wrong message format \""+msg+"\"";
    }
#ifdef BT_TEST
    std::cout << "znaleziono " << var <<" na pozycji " << pos <<std::endl;
#endif

    for (int i = 1+pos+var.size();;++i ){
        t = msg.at(i);
        if (t ==';'){
            break;
        }
        id += t;
    }
    return id;

}