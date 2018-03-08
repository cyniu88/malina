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

    serial_RFLink.begin( std::stoi(my_data->server_settings->RFLinkBaudRate));
#ifndef BT_TEST
    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"otwarcie portu RS232 RFLink " << my_data->server_settings->RFLinkPort << "  "
                  <<my_data->server_settings->RFLinkBaudRate<<std::endl;
    log_file_mutex.mutex_unlock();
#endif

    return true;

}

void RFLinkHandler::run()
{
    std::string msg;
    while (useful_F::go_while){
        std::this_thread::sleep_for( std::chrono::milliseconds(50));
        {
            std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);

            msg = readFromRS232();

            puts("odebrane od RFLinka:");
            puts(msg.c_str());
            //TODO  dodac obsluge;
        }
    }
}

void RFLinkHandler::sendCommand(std::string cmd)
{
    std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);
    serial_RFLink.print(cmd.c_str());
}

std::string RFLinkHandler::sendCommandAndWaitForReceive(std::string cmd)
{
    std::lock_guard<std::mutex> m_lock(sm_RFLink_MUTEX);
    serial_RFLink.print(cmd.c_str());
    return readFromRS232();
}

std::string RFLinkHandler::readFromRS232()
{
    std::string buf;
    char b;

    while (serial_RFLink.available() > 0){
        b = serial_RFLink.read();

        if (b == '\n'){
            break;
        }
        buf += b;
    }
    return buf;
}
