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
    std::cout << " wielkosc bufora "<< strlen(c_buffer) << std::endl;
    if(( send( c_socket, c_buffer ,strlen(c_buffer), para ) ) <= 0 )
    {
        //perror( "send() ERROR" );
        return -1;
    }

    return 0;
}

int C_connection::c_send(char command[MAX_buf])
{
    c_write_buf(command);
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
            c_write_buf("\nEND.\n");
            break;
        }
        else if (command[0]=="hello")
        {
            c_write_buf("\nHI !\n");
            break;
        }
        else if (command [0] == "help")
        {
            c_write_buf("\n exit \t- close client \n stop server \t- stop server iDom and close \n RS232 get temperature \t - get temperature from room \nhello - test connection \n");
            break;
        }
        else if (command [0] == "OK")
        {
            c_write_buf("\nEND.\n");
        break;
        }
        else if (command [0] == "IP")
        {
            c_write_buf(( char*) my_data->server_settings->SERVER_IP.c_str() );
        break;
        }
        else
        {
           c_write_buf("\nEND.\n");
           break;
        }
    case 2 :
        if(command[0]=="stop")
        {
            std::cout << " jest stop";
            if (command[1]=="server")
            {
                std::cout <<"wychodze!!!!";
                c_send("\nEND.\n");
                return false;

            }
            break;
        }
        else if (command[0]=="show")
        {
            if (command[1]=="log")
            {

                l_send_log();
            }
        }
        else
        {
           c_write_buf("\nEND.\n");
           break;
        }


    case 3:

        if (command[0]=="RS232")
        {
            if (command[1]=="get")
            {
                if (command[2]=="temperature"){
                    std::cout << " szukam temeratury" << std::endl;
                    while (go_while)
                    {
                        usleep(500);
                        pthread_mutex_lock(&C_connection::mutex_who);
                        if (my_data->pointer.ptr_who[0] == FREE)
                        {
                            pthread_mutex_lock(&C_connection::mutex_buf);


                            my_data->pointer.ptr_who[0]=RS232;
                            my_data->pointer.ptr_who[1]= pthread_self();
                            buffer=command[2];

                            pthread_mutex_unlock(&C_connection::mutex_buf);
                            pthread_mutex_unlock(&C_connection::mutex_who);
                            break;
                        }
                        pthread_mutex_unlock(&C_connection::mutex_who);

                    }

                    while (go_while)
                    {
                        usleep(500);
                        pthread_mutex_lock(&C_connection::mutex_who);
                        if (my_data->pointer.ptr_who[0] == pthread_self())
                        {
                            pthread_mutex_lock(&C_connection::mutex_buf);


                            my_data->pointer.ptr_who[0]=FREE;
                            my_data->pointer.ptr_who[1]= 0;
                            //buffer +=" taka temeratura";

                            c_write_buf(  (char *) buffer.c_str()    );

                            pthread_mutex_unlock(&C_connection::mutex_buf);
                            pthread_mutex_unlock(&C_connection::mutex_who);
                            break;
                        }
                        pthread_mutex_unlock(&C_connection::mutex_who);

                    }



                    break;
                }
                break;
            }
            break;
        }



        else
        {
           c_write_buf("END.\n");
           break;
        }

    default :
        std::cout << " nic nie przyszlo komenda z dupy " << c_buffer<<std::endl;
        c_write_buf("unknown command\n");

    }


    return true;
}

void C_connection::c_send_recv_MASTER()
{


}



void C_connection::c_send_recv_RS232()
{   int test = RS232;

}


