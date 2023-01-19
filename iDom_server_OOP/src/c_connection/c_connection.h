#ifndef C_CONNECTION_H
#define C_CONNECTION_H

#include <mutex>
#include <unistd.h>
#include <iostream>

#include "../blockQueue/blockqueue.h"
#include "../command/commandhandlerroot.h"
#include "../iDom_server_OOP.h"

constexpr int MAX_buf = 32768;

class C_connection : public iDom_API
{
#ifdef BT_TEST
   friend class c_connection_fixture;
#endif
public:
    explicit C_connection(thread_data *my_data);
    ~C_connection();

    thread_data *my_data;
    int c_socket = 0;
    sockaddr_in c_from;

    char c_buffer[MAX_buf];
    int m_recv_size;

    int c_send(int para);
    int c_send(const std::string& command);
    int c_sendPure(const std::string& command);
    int c_recv(int para);
    std::string c_read_buf(int recvSize);
    void c_analyse(int recvSize);
    void setEncriptionKey(const std::string& key);
    void setEncrypted(bool flag);
    std::unique_ptr<commandHandler> m_mainCommandHandler = std::nullptr_t();
    void onStartConnection();
    void onStopConnection();
    void cryptoLog(std::string &toEncrypt);
    void hendleHTTP(const std::string& msg);
#ifdef BT_TEST
    std::string getStr_buf(){
        return m_str_buf;
    }
#endif
    std::string dump() const override;
private:
    std::string m_str_buf;
    blockQueue m_char_queue;
    int m_counter = 0;
    std::string m_encriptionKey;
    bool m_encrypted;
    void crypto(std::string &toEncrypt, std::string key, bool encrypted);
    C_connection();
};

#endif // C_CONNECTION1_H
