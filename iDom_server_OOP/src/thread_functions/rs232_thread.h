#ifndef RS232_THREAD_H
#define RS232_THREAD_H

#include "../c_connection/c_connection.h"
#include "../TASKER/tasker.h"



//////////// watek wysylajacy/obdbierajacy dane z portu RS232 ////////
void Send_Recieve_rs232_thread (thread_data_rs232 *data_rs232){

    SerialPi serial_ardu(strdup( data_rs232->portRS232.c_str()));
    serial_ardu.begin( std::stoi( data_rs232->BaudRate));
#ifndef BT_TEST
    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"otwarcie portu RS232 " <<  data_rs232->portRS232 << "  " <<data_rs232->BaudRate<<std::endl;
    log_file_mutex.mutex_unlock();
#endif
    SerialPi serial_ardu_clock(strdup( data_rs232->portRS232_clock.c_str()));
    serial_ardu_clock.begin( std::stoi( data_rs232->BaudRate));
#ifndef BT_TEST
    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"otwarcie portu RS232_clock " <<  data_rs232->portRS232_clock <<" "<< data_rs232->BaudRate <<std::endl;
    log_file_mutex.mutex_unlock();
#endif
    /////////////////////////////////////////////////// RESET ARDUINO AFTER RESTART ////////////////////////////////
    puts("restart arduino\n");
    //C_connection::mutex_who.lock();
    {
        std::lock_guard<std::mutex> lockWho(useful_F::mutex_who);
        buffer = "reset:00;";
        serial_ardu.print(buffer.c_str());
    }
    //C_connection::mutex_who.unlock();
    puts("test testo po lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(useful_F::go_while)
    {
        //puts("test");
        std::this_thread::sleep_for( std::chrono::milliseconds(50));
        //puts("test testo po lock");
        { //mutex who
            std::lock_guard<std::mutex> lockWho(useful_F::mutex_who);

            if(data_rs232->pointer.ptr_who[0] == iDomConst::RS232)
            {
                std::lock_guard<std::mutex> lockBuf(useful_F::mutex_buf);
                data_rs232->pointer.ptr_who[0] = data_rs232->pointer.ptr_who[1];
                data_rs232->pointer.ptr_who[1] = iDomConst::RS232;
                serial_ardu.print(buffer.c_str());

                buffer.erase();

                while(useful_F::go_while){
                    if(serial_ardu.available()>0){
                        buffer += serial_ardu.read();
                    }
                    if(buffer[buffer.size()-1] == ';')
                    {
                        buffer.erase(buffer.end()-1);
                        break;
                    }
                }
            }
            else if(data_rs232->pointer.ptr_who[0] == iDomConst::CLOCK)
            {
                std::lock_guard<std::mutex> lockBuf(useful_F::mutex_buf);
                data_rs232->pointer.ptr_who[0] = data_rs232->pointer.ptr_who[1];
                data_rs232->pointer.ptr_who[1] = iDomConst::RS232;
                serial_ardu_clock.print(buffer.c_str());

                buffer.erase();

                while(useful_F::go_while){
                    if(serial_ardu_clock.available()>0){
#ifdef BT_TEST
                        std::cout << "w buforze serial_ardu_clock " << serial_ardu_clock.available()<<std::endl;
#endif
                        buffer += serial_ardu_clock.read();
                        buffer += serial_ardu_clock.read();
                        serial_ardu_clock.flush();
                        break;
                    }
                }
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
                        // std::cout << "serial_ardu.available(): "<<serial_ardu.available()<<std::endl;
                        if(serial_ardu.available()>0){
                            char t = serial_ardu.read();
                            // std::cout << "t: "<<t<<std::endl;
                            if(t == ';'){
                                serial_ardu.flush();

                                break;
                            }
                            else{
                                printf("%c",t);
                                bufor.push_back(t);
                            }
                        }
                    }
                    useful_F::myStaticData->myEventHandler.run("RS232")->addEvent(bufor);

                }
            }

        }
    }
}
#endif // RS232_THREAD_H
