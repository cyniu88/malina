#ifndef C_CONNECTION_H
#define C_CONNECTION_H
# include <mutex>
#include <unistd.h>
#include <iostream>
#include "../functions/functions.h"                 // brak
#include "../CRON/cron.hpp"
#include "../functions/mpd_cli.h"
#include "../command/commandhandlerroot.h"
#include "../iDom_server_OOP.h"


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
    int recv_size;
    struct s_pointer *pointer;

    int c_send(int para);
    int c_send(std::string command);
    int c_recv(int para);
    void c_send_recv_RS232 ();
    std::string c_read_buf (int recvSize);
    void c_send_recv_MASTER ();
    int c_analyse(int recvSize);
    void setEncriptionKey(std::string key);
    void setEncrypted(bool flag);
    commandHandler *mainCommandHandler;
    void onStartConnection();
    void onStopConnection();

private:
    std::string  str_buf;
    blockQueue char_queue;
    int counter = 0;
    std::string encriptionKey;
    bool m_encrypted;
    void crypto(std::string &toEncrypt, std::string key, bool encrypted);

};

#endif // C_CONNECTION_H
