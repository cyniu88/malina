#include "c_connection.h"
#include <iostream>

C_connection::C_connection (thread_data  *my_data):c_socket(my_data->s_client_sock),
    c_from(my_data->from),recv_size(0)
{
    this -> pointer = &my_data->pointer;
    this -> my_data = my_data;
    this->m_encrypted = my_data->server_settings->encrypted;
    std::fill(std::begin(c_buffer),std::end(c_buffer),',');
    onStartConnection();
}

C_connection::~C_connection()
{
    if( mainCommandHandler != NULL){
        my_data->mainLCD->set_print_song_state(0);
        my_data->mainLCD->set_lcd_STATE(2);
        delete mainCommandHandler;
    }
    my_data->mainLCD->set_print_song_state(0);
    my_data->mainLCD->set_lcd_STATE(2);
    sleep (3);
    shutdown( c_socket, SHUT_RDWR );
    useful_F::clearThreadArray(my_data);
    puts("C_connection::~C_connection()");
}


int C_connection::c_send(int para)
{
    crypto(str_buf,m_encriptionKey,m_encrypted); //BUG  - naprawic czytanie flagi  z parametru klasy
    std::string  len = std::to_string( str_buf.size());
    crypto(len,m_encriptionKey,m_encrypted);
    if(( send( c_socket, len.c_str() ,len.length(), para ) ) <= 0 )
    {
        return -1;
    }
    recv_size = recv( c_socket, c_buffer , MAX_buf, para );

    if(recv_size < 0 )
    {
        //perror( "recv() ERROR" );
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "C_connection::c_send(int para) recv() error - " << strerror(  errno ) <<   std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }
    else if (recv_size == 0)
    {
        return -1;
    }

    auto len_send = str_buf.length();

    while (len_send > 0)
    {
        auto len_temp = send( c_socket, str_buf.c_str() ,str_buf.length(), para ) ;
        if(len_temp  <= 0 )
        {
            return -1;
        }
        len_send -= len_temp;
        str_buf.erase(0,len_temp);
    }
    return 0;
}

int C_connection::c_send(std::string command )
{
    str_buf = command;
    //crypto(str_buf,encriptionKey);
    return c_send(0);
    //return 0;
}

int C_connection::c_recv(int para)
{
    struct timeval tv;
    tv.tv_sec = 90;
    tv.tv_usec = 0;
    setsockopt(c_socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv , sizeof(struct timeval));

    recv_size = recv( c_socket, c_buffer , MAX_buf, para );

    if(recv_size < 0 )
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "C_connection::c_recv(int para) recv() error - " << strerror(  errno ) <<   std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }
    else if (recv_size == 0)
    {
        //        log_file_mutex.mutex_lock();
        //        log_file_cout << INFO << "gniazdo zamkniete" <<   std::endl;
        //        log_file_mutex.mutex_unlock();
        return -1;
    }
    return recv_size;
}

int C_connection::c_analyse(int recvSize)
{
    std::string buf;

    //    for (int i = 0 ; i < recvSize; ++i){
    //        buf+= c_buffer[i];
    //    }
    buf = c_read_buf(recvSize);
    my_data->myEventHandler.run("command")->addEvent(buf);
    std::vector <std::string> command;
    useful_F::tokenizer(command," \n,", buf);  // podzia  zdania na wyrazy
    str_buf = command[command.size()-1];
    str_buf = "unknown command\n";

    for(std::string  t : command)
    {
        str_buf += t+" ";
    }

    str_buf  = mainCommandHandler->run(command,my_data);

    return true;
}
