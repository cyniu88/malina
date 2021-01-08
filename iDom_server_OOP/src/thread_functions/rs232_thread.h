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

    /////////////////////////////////////////////////// RESET ARDUINO AFTER RESTART ////////////////////////////////
    puts("restart arduino\n");
    {
        std::lock_guard<std::mutex> lockWho(useful_F::mutex_who);
        buffer = "reset:00;";
        serial_ardu.print(buffer.c_str());
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(useful_F::go_while)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(50));
        { //mutex who
            std::lock_guard<std::mutex> lockWho(useful_F::mutex_who);

            if(data_rs232->pointer.ptr_who[0] == iDomConst::RS232)
            {
                std::lock_guard<std::mutex> lockBuf(useful_F::mutex_buf);
                data_rs232->pointer.ptr_who[0] = data_rs232->pointer.ptr_who[1];
                data_rs232->pointer.ptr_who[1] = iDomConst::RS232;
                serial_ardu.print(buffer.c_str());

                buffer.erase();

#ifdef BT_TEST
                useful_F::go_while = false;
                return;
#endif
            }
            else if(data_rs232->pointer.ptr_who[0] == iDomConst::FREE)
            {
                std::string bufor = "";
                if(serial_ardu.available()>0) {

                    while (useful_F::go_while){
                        if(serial_ardu.available()>0){
                            char t = serial_ardu.read();
                            if(t == ';'){
                                serial_ardu.flush();
                                break;
                            }
                            else{
                                bufor.push_back(t);
                            }
                        }
                    }
                    useful_F::myStaticData->myEventHandler.run("RS232")->addEvent(bufor);
                }
#ifdef BT_TEST
                useful_F::go_while = false;
                return;
#endif
            }
        }
    }
    iDOM_THREAD::stop_thread(threadName, useful_F::myStaticData);
}
#endif // RS232_THREAD_H
