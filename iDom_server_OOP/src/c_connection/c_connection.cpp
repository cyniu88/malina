#include <iostream>
#include "c_connection.h"
#include "../thread_functions/iDom_thread.h"
#include "../functions/functions.h"

C_connection::C_connection (thread_data *my_data):c_socket(my_data->s_client_sock),
    c_from(my_data->from),m_recv_size(0)
{
    this -> pointer = &my_data->pointer;
    this -> my_data = my_data;
    this->m_encrypted = my_data->server_settings->_server.encrypted;
    std::fill(std::begin(c_buffer),std::end(c_buffer),',');
    onStartConnection();
    m_className.append(typeid(this).name());
    m_className.append(" ");
    m_className.append(std::to_string(c_socket));
    iDom_API::addToMap(m_className,this);
}

C_connection::~C_connection()
{
    iDom_API::removeFromMap(m_className);
    if( m_mainCommandHandler != std::nullptr_t())
    {
        my_data->mainLCD->set_print_song_state(0);
        my_data->mainLCD->set_lcd_STATE(2);
        delete m_mainCommandHandler;
    }
    my_data->mainLCD->set_print_song_state(0);
    my_data->mainLCD->set_lcd_STATE(2);

    useful_F::sleep(3);

    shutdown(c_socket, SHUT_RDWR );
    puts("C_connection::~C_connection()");
}

int C_connection::c_send(int para)
{
    crypto(m_str_buf,m_encriptionKey,m_encrypted);
    std::string len = std::to_string( m_str_buf.size());
    crypto(len,m_encriptionKey,m_encrypted);
    if(( send( c_socket, len.c_str() ,len.length(), para ) ) <= 0 )
    {
        return -1;
    }
    m_recv_size = recv( c_socket, c_buffer , MAX_buf, para );

    if(m_recv_size < 0 )
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "C_connection::c_send(int para) recv() error - " << strerror( errno ) << std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }
    else if (m_recv_size == 0)
    {
        return -1;
    }

    auto len_send = m_str_buf.length();

    while (len_send > 0)
    {
        auto len_temp = send( c_socket, m_str_buf.c_str() ,m_str_buf.length(), para );
        if(len_temp <= 0 )
        {
            return -1;
        }
        len_send -= len_temp;
        m_str_buf.erase(0,len_temp);
    }
    return 0;
}

int C_connection::c_send(const std::string &command )
{
    m_str_buf = command;
    return c_send(0);
}

int C_connection::c_recv(int para)
{
    struct timeval tv;
    tv.tv_sec = 90;
    tv.tv_usec = 0;
    setsockopt(c_socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv , sizeof(struct timeval));

    m_recv_size = recv(c_socket, c_buffer, MAX_buf, para);

    if(m_recv_size < 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "C_connection::c_recv(int para) recv() error - " << strerror( errno ) << std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }
    else if (m_recv_size == 0)
    {
        return -1;
    }
    return m_recv_size;
}

void C_connection::c_analyse(int recvSize)
{
    std::string buf;

    buf = c_read_buf(recvSize);
    my_data->myEventHandler.run("command")->addEvent(buf);
    std::vector <std::string> command;
    try{
        useful_F::tokenizer(command," \n,", buf);
    }
    catch (std::string& k){
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "brak komendy - " << k << std::endl;
        log_file_mutex.mutex_unlock();
        m_str_buf = "empty command";
        return;
    }

#ifdef BT_TEST
    std::cout << "komenda: " << m_str_buf << " command.size() " << command.size() << std::endl;

    m_str_buf = "unknown command\n";

    for(std::string t : command)
    {
        m_str_buf += t+" ";
    }
#endif
    m_str_buf = m_mainCommandHandler->run(command,my_data);
}


std::string C_connection::dump() const
{
    std::stringstream ret;

    ret << " c_from.sin_addr.s_addr: " <<  this->c_from.sin_addr.s_addr << std::endl;
    ret << " c_socket: " <<  this->c_socket << std::endl;
    ret << " m_recv_size: " <<  this->m_recv_size << std::endl;
    ret << " m_className: " <<  this->m_className << std::endl;
    ret << " m_counter: " <<  this->m_counter << std::endl;
    ret << " m_encriptionKey: " <<  this->m_encriptionKey << std::endl;
    ret << " m_encrypted: " <<  this->m_encrypted << std::endl;
    ret << " m_str_buf: " <<  this->m_str_buf << std::endl;

    return ret.str();
}
