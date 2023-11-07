#ifndef RS232_THREAD_H
#define RS232_THREAD_H

#include "../SerialPi/serialpi.h"
#include "../thread_functions/iDom_thread.h"
#include "../functions/functions.h"

using namespace std::chrono_literals;

//////////// watek wysylajacy/obdbierajacy dane z portu RS232 ////////
void Send_Recieve_rs232_thread(thread_data *context, const std::string &threadName)
{
    context->main_Rs232 = std::make_unique<SerialPi>(context->server_settings->_rs232.portRS232);
    context->main_Rs232->begin(context->server_settings->_rs232.BaudRate);
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "otwarcie portu RS232 " << context->server_settings->_rs232.portRS232 << " " << context->server_settings->_rs232.BaudRate << std::endl;
    log_file_mutex.mutex_unlock();
    std::string buffor;

    while (useful_F::go_while)
    {
        if (context->main_Rs232->available() > 0)
        {
            char t = context->main_Rs232->read();
            if (t == '\n')
            {
                context->main_Rs232->flush();
                auto data = useful_F::split(buffor, ':');
                if (data.at(0) == "KEY_PAD")
                {
                    int id = std::stoi(data.at(1));
                    KEY_PAD keyEvent = static_cast<KEY_PAD>(id);
                    context->main_key_menu_handler->recKeyEvent(keyEvent);
                }
                else if (data.at(0) == "TIMEOUT")
                {
                    context->main_key_menu_handler->timeout();
                }
                else
                {
                    context->mqttHandler->publish("rs232", buffor);
                }
                buffor.clear();
            }
            else
            {
                buffor.push_back(t);
            }
        }
        std::this_thread::sleep_for(50ms);
    }
    std::this_thread::sleep_for(1.5s);
    context->main_Rs232->flush();
    iDOM_THREAD::stop_thread(threadName, context);
}
#endif // RS232_THREAD_H
