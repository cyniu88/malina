#include "c_connection.h"
#include <iostream>
//using namespace std;



pthread_mutex_t C_connection::mutex_buf = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t C_connection::mutex_who = PTHREAD_MUTEX_INITIALIZER;

// konstruktor
C_connection::C_connection (thread_data  *my_data):c_socket(my_data->s_client_sock),
    c_from(my_data->from)
{
    //c_max_msg = MAX_MSG_LEN*sizeof(int32_t);
    //c_from= my_data->from;
    // c_socket= my_data->s_client_sock;
    this -> pointer = &my_data->pointer;
    this -> my_data = my_data;
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< " konstruuje nowy obiekt do komunikacj " <<  std::endl;
    log_file_mutex.mutex_unlock();
    c_help = "\n exit - end program \n stop server - stop server iDom and exit \n help - print command ";
}



// destruktor
C_connection::~C_connection()
{
    shutdown( c_socket, SHUT_RDWR );
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< " koniec komunikacji - kasuje obiekt" <<  std::endl;
    log_file_mutex.mutex_unlock();
}

int C_connection::c_send(int para)
{
    // ChangeEndianness(msg.c_bufor_tmp,MAX_MSG_LEN);
    if(( send( c_socket, c_buffer , MAX_buf, para ) ) <= 0 )
    {
        //perror( "send() ERROR" );
        return -1;
    }

    return 0;
}

int C_connection::c_send(char command[MAX_buf])
{
    c_write_buf("end");
    c_send(0);
    return 0;
}

int C_connection::c_recv(int para)
{
    for (unsigned int i =0 ; i< sizeof(c_buffer);++i)
    {
        c_buffer[i]=' ';
    }
    recv_size = recv( c_socket, c_buffer , MAX_buf, para );
    if(recv_size < 0 )
    {
        //perror( "recv() ERROR" );
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << " recv() error" <<   std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }
    else if (recv_size == 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "gniazdo zamkniete" <<   std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }

    std::cout << "giazdo odebralo bajtow: " <<recv_size << std::endl;
    for (unsigned int i =0 ; i< recv_size;++i)
    {
        std::cout << c_buffer[i];
    }
    std::cout << " " <<std::endl;
    return 1;
}






int C_connection::c_analyse()
{

    std::string buf(c_buffer);
    std::vector <std::string> command;

    boost::char_separator<char> sep(",:\n ");
    boost::tokenizer< boost::char_separator<char> > tokens(buf, sep);
    BOOST_FOREACH (const string& t, tokens) {
        std::cout << " rozmiar t: " << t.size() << std::endl;
        command.push_back( t);
    }
    command.pop_back();  // usowa ostanit wpis smiec


    switch (command.size())
    {
    case 1 :
        if(command[0]=="exit")
        {
            std::cout << " klient sie odlaczyl" << std::endl;
            c_write_buf("bye");
            break;
        }
        else if (command[0]=="hello")
        {
            c_write_buf("HI !");
            break;
        }
        else if (command [0] == "help")
        {
            c_write_buf(&c_help[0u]);
            break;
        }
        break;
    case 2 :
        if(command[0]=="stop")
        {
            std::cout << " jest stop";
            if (command[1]=="server")
            {
                std::cout <<"wychodze!!!!";
                c_send("end");
                return false;

            }
        }
        break;
      default :
        c_write_buf("unknow command");

    }


    return true;
}

void C_connection::c_send_recv_MASTER()
{


}



void C_connection::c_send_recv_RS232()
{   int test = RS232;

}


