#ifndef C_CONNECTION_H
#define C_CONNECTION_H

#include "../functions/functions.h"                 // brak
#include "../CRON/cron.hpp"
#include "../functions/mpd_cli.h"
//#include "../iDom_server/src/iDom_server.h"

#include <unistd.h>
//#include <boost/foreach.hpp>
//#include <boost/tokenizer.hpp>
#include <iostream>

constexpr int MAX_buf = 32768;
class C_connection
{
public:
    C_connection(thread_data  *my_data);
    ~C_connection();

    thread_data   *my_data;
    int c_socket;

    sockaddr_in c_from;

    char  c_buffer[MAX_buf];
    //char c_buffer[MAX_buf];
    // std::string  str_buf;
    int recv_size;


    static pthread_mutex_t mutex_buf ;// = PTHREAD_MUTEX_INITIALIZER;
    static pthread_mutex_t mutex_who ;//= PTHREAD_MUTEX_INITIALIZER;
    struct s_pointer *pointer;


    int c_send(int para);
    int c_send(std::string msg,int para);
    int c_send(std::string command);
    int c_recv(int para);
    //int32_t c_return(int iterator);
    void c_get(int32_t buffor, int i);
    void c_send_recv_RS232 ();
    std::string c_read_buf ( );
    //void c_write_buf(char *buf);
    void c_send_recv_MASTER ();
    int c_analyse();
    bool c_analyse_exit();
    void c_set_buf (int what);


    // metody dla noda
    void c_start_master();
    void c_recv_send_master();

    // wysylanie loga
    void l_send_file(std::string path, std::string find="", bool reverse= false);

private:
    //std::string temporary_str;
    std::string  str_buf;
    blockQueue char_queue;
    int  counter =0;
};


#endif // C_CONNECTION_H
