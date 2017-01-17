
#include "c_connection.h"
#include <iostream>


void C_connection::c_start_master ()
{

    std::cout <<  " w pamieci jest " <<my_data->server_settings->AAS.size() <<" nodow\n";
    for (unsigned int i =0 ; i<my_data->server_settings->AAS.size() ; ++i )
    {
        std::cout << "  jest ustawien id " <<  my_data->server_settings->AAS[i].id << std::endl;
        std::cout << "  jest ustawien ip " <<  my_data->server_settings->AAS[i].SERVER_IP << std::endl;
    }
        while (1)
        {

             //c_recv_master_to();
             c_recv_send_master();
             std::cout << " odebralem w masterze \n";
             //c_send_master_to(c_bufor_tmp[16]);

        }


}


void C_connection::c_recv_send_master()
{
    while (1)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(50) );

        mutex_who.lock();
        if (  pointer->ptr_who[0] == my_data->server_settings->ID_server)
        {
            mutex_buf.lock();

            for (int i =0 ; i < MAX_MSG_LEN ; ++i )
            {
                 pointer->ptr_buf[i]-=1;
            }
            pointer->ptr_who[0] =  pointer->ptr_who[1] ;
            pointer->ptr_who[1] =  my_data->server_settings->ID_server;
            mutex_buf.unlock();
        }
         mutex_who.unlock();
    } // end while
}

void C_connection::setEncriptionKey(std::string key)
{
     encriptionKey = key;
}
