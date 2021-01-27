#ifndef RS232_THREAD_H
#define RS232_THREAD_H

#include "../c_connection/c_connection.h"
#include "../TASKER/tasker.h"
#include "../SerialPi/serialpi.h"
#include "../thread_functions/iDom_thread.h"
#include "../functions/functions.h"
#include "../MENU/menu_base.h"

//////////// watek wysylajacy/obdbierajacy dane z portu RS232 ////////
void Send_Recieve_rs232_thread (thread_data *my_data, const std::string& threadName){
    my_data->main_Rs232 = std::make_unique<SerialPi>(my_data->server_settings->_rs232.portRS232);
    my_data->main_Rs232->begin(my_data->server_settings->_rs232.BaudRate);
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "otwarcie portu RS232 " << my_data->server_settings->_rs232.portRS232 <<
                     " " << my_data->server_settings->_rs232.BaudRate << std::endl;
    log_file_mutex.mutex_unlock();
    std::string buffor;

    while(useful_F::go_while)
    {
        if(my_data->main_Rs232->available() > 0){
            char t = my_data->main_Rs232->read();
            if(t == '\n'){
                my_data->main_Rs232->flush();
                auto data = useful_F::split(buffor,':');
                if(data.at(0) == "KEY_PAD"){
                    int id = std::stoi(data.at(1));
                    KEY_PAD keyEvent = static_cast<KEY_PAD>(id);
                    my_data->main_key_menu_handler->recKeyEvent(keyEvent);
                }
                else if(data.at(0) == "TIMEOUT"){
                    my_data->main_key_menu_handler->timeout();
                }
                buffor.clear();
            }
            else{
                buffor.push_back(t);
            }
        }
        std::this_thread::sleep_for( std::chrono::milliseconds(50));
    }
    std::this_thread::sleep_for( std::chrono::milliseconds(1500));
    my_data->main_Rs232->flush();
    iDOM_THREAD::stop_thread(threadName, my_data);
}
#endif // RS232_THREAD_H
