
#include "c_connection.h"
#include <iostream>

void C_connection::c_write_buf(char * buf){

    for (unsigned int i =0 ; i< sizeof(c_buffer);++i)
    {
        c_buffer[i]=' ';
    }
    for (unsigned int i =0 ; i< sizeof(buf);++i)
    {
        c_buffer[i]=buf[i];
    }


}


void C_connection::c_read_buf(int c_pthread_self)
{



}

void C_connection::l_send_log(){
//   // conv msg;
//    std::fstream log_file;
//    log_file.open(_logfile,std::ios::in);
//    if( log_file.good() == false )
//    {
//        // tu wpisac wysylanie ze problem  z otwarciem
//        return;
//    }


//    else
//    {

//        while( !log_file.eof() )
//        {

//            for (int i =4 ; i<4*MAX_MSG_LEN ; ++i)
//            {
//                if (log_file.eof()==1)
//                {
//                    msg.char_buf[i-1]= 10;   // pozbywami się -1 z konca pliku txt
//                    break;
//                }
//                msg.char_buf[i]= log_file.get();
//            }
//            //std::cout << "******************************\n";
////            for (int i =1 ; i<4*MAX_MSG_LEN ; ++i)
////            {
////                std::cout << msg.char_buf[i] ;
////            }
//            ChangeEndianness(msg.c_bufor_tmp,MAX_MSG_LEN);
//            c_send(0);
//            c_recv(0);
//        }
//       // std::cout << " wysylam ramke koniec transmisji pliku " << std::endl;
//        c_set_buf(-94);
//       // std::cout << " wyslalem przed konwersja" << msg.c_bufor_tmp[0] <<std::endl;
//        //c_send(0);
//       // std::cout << " wyslalem " << msg.c_bufor_tmp[0] <<std::endl;
//       // c_recv(0);
//    }


//    log_file.close();
}







