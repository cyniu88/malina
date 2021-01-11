#ifndef RS232_THREAD_H
#define RS232_THREAD_H

#include "../c_connection/c_connection.h"
#include "../TASKER/tasker.h"
#include "../SerialPi/serialpi.h"
#include "../thread_functions/iDom_thread.h"
#include "../functions/functions.h"

//////////// watek wysylajacy/obdbierajacy dane z portu RS232 ////////
void Send_Recieve_rs232_thread (thread_data_rs232 *data_rs232, const std::string& threadName){

    SerialPi serial_ardu(data_rs232->portRS232);
    serial_ardu.begin( data_rs232->BaudRate);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"otwarcie portu RS232 " << data_rs232->portRS232 << " " <<data_rs232->BaudRate<<std::endl;
    log_file_mutex.mutex_unlock();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(useful_F::go_while)
    {

    }
    iDOM_THREAD::stop_thread(threadName, useful_F::myStaticData);
}
#endif // RS232_THREAD_H
