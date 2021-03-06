#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>

#include "../satel_integra_handler.h"

static bool workStubSatel = true;
void satelServer(){
    std::cout << "start" << std::endl;
    int  MAX_buf = 1024;
    char c_buffer[MAX_buf];
    struct sockaddr_in server;
    int v_socket;
    int SERVER_PORT = 7094;
    const char *SERVER_IP = "127.0.0.1";

    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, SERVER_IP, & server.sin_addr) <= 0)
    {
        perror("inet_pton() ERROR");
        exit(-1);
    }

    if((v_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket() ERROR");
        exit(-1);
    }

    if(fcntl(v_socket, F_SETFL, O_NONBLOCK) < 0) // fcntl()
    {
        perror("fcntl() ERROR");
        exit(-1);
    }
    // zgub wkurzający komunikat błędu "address already in use"
    int yes = 1;
    if(setsockopt(v_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == - 1) {
        perror("setsockopt");
        exit(1);
    }
    socklen_t len = sizeof(server);
    if(bind(v_socket,(struct sockaddr *) & server, len) < 0)
    {
        perror("bind() ERROR");
        exit(-1);
    }

    if(listen(v_socket, iDomConst::MAX_CONNECTION) < 0)
    {
        perror("listen() ERROR");
        exit(-1);
    }
    struct sockaddr_in from;

    std::cout << " przed while " << std::endl;
    while (workStubSatel)
    {
        int v_sock_ind = 0;
        memset(&from,0, sizeof(from));
        //  if(!useful_F::workServer) {
        //      break;
        //  }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if((v_sock_ind = accept(v_socket,(struct sockaddr *) & from, & len)) < 0)
        {
            continue;
        }
        else{
            std::cout << "polaczono " << std::endl;
            while(workStubSatel){
                auto  m_recv_size = recv(v_sock_ind, c_buffer, MAX_buf, 0);
                if (m_recv_size > 0){
                    if(c_buffer[2] == INTEGRA_ENUM::VERSION){
                        std::cout << "wersja integry" << std::endl;
                        std::string len;
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back('3');
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::END);
                        int siz = send( v_sock_ind, len.c_str() ,len.length(), 0);
                        if( siz >= 0 )
                        {
                            std::cout << "wyslano " << siz << std::endl;
                        }
                        else
                            std::cout << " nie wyslano " << siz << std::endl;
                    }
                    else if(c_buffer[2] == INTEGRA_ENUM::OUTPUTS_STATE){
                        std::cout << "stan wejsc integry" << std::endl;
                        std::string len;
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::OUTPUTS_STATE);
                        len.push_back(0x3);
                        len.push_back(0x4);
                        len.push_back(0x5);
                        len.push_back(0x6);
                        len.push_back('3');
                        len.push_back('3');

                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::END);

                        int siz = send( v_sock_ind, len.c_str() ,len.length(), 0);
                        if( siz >= 0 )
                        {
                            std::cout << "wyslano "<< std::endl;
                        }
                        else
                            std::cout << " nie wyslano" << std::endl;
                    }
                    else if(c_buffer[2] == (char)INTEGRA_ENUM::ARM){
                        std::cout << "uzbrojenie alarmu "  << (int)c_buffer[3] << " " << (int)c_buffer[4] << std::endl;
                        std::string len;
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::ARM);
                        len.push_back(0x3);
                        len.push_back(0x4);
                        len.push_back(0x5);
                        len.push_back(0x6);
                        len.push_back('3');
                        len.push_back('3');

                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::END);

                        int siz = send( v_sock_ind, len.c_str() ,len.length(), 0);
                        if( siz >= 0 )
                        {
                            std::cout << "wyslano "<< std::endl;
                        }
                        else
                            std::cout << " nie wyslano" << std::endl;
                    }
                    else if(c_buffer[2] == INTEGRA_ENUM::ARMED_PARTITIONS){
                        std::cout << "stan alarmu integry" << std::endl;
                        std::string len;
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::ARMED_PARTITIONS);
                        len.push_back(0x0);
                        len.push_back('3');
                        len.push_back('3');

                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::END);

                        int siz = send( v_sock_ind, len.c_str() ,len.length(), 0);
                        if( siz >= 0 )
                        {
                            std::cout << "wyslano "<< std::endl;
                        }
                        else
                            std::cout << " nie wyslano" << std::endl;
                    }
                }

            }
        }



    } // while
    std::cout <<  "zamykamy stub serwera integra" << std::endl;
    close(v_socket);
}


class satel_integra_fixture: public testing::Test
{
public:
    satel_integra_fixture() = default;
    void startSatelServer(){
        workStubSatel = true;
        auto t = std::thread(satelServer);
        t.detach();
        std::cout << "EXIT bit_fixture::start_iDomServer()" << std::endl;
    }

protected:

    void SetUp()
    {
        std::cout << "satel_integra_fixture SetUp()" << std::endl;
    }
    void TearDown()
    {
        workStubSatel = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "satel_integra_fixture TearDown()" << std::endl;
    }

};

TEST_F(satel_integra_fixture, checkIntegraOut)
{
    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    struct CONFIG_JSON test_config;
    test_config._satel_integra.host = "127.0.0.1";
    test_config._satel_integra.port = 7094;
    test_config._satel_integra.pin = "1234";
    thread_data test_threadData;
    test_threadData.server_settings = &test_config;

    SATEL_INTEGRA_HANDLER testIntegra(&test_threadData);
    testIntegra.checkSatel();
    testIntegra.m_integra32.getIntegraInfo();
    EXPECT_FALSE(testIntegra.m_integra32.isAlarmArmed());
}

TEST_F(satel_integra_fixture, main)
{
    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    struct CONFIG_JSON test_config;
    test_config._satel_integra.host = "127.0.0.1";
    test_config._satel_integra.port = 7094;
    test_config._satel_integra.pin = "1234";
    thread_data test_threadData;
    test_threadData.server_settings = &test_config;

    SATEL_INTEGRA_HANDLER testIntegra(&test_threadData);
    testIntegra.checkSatel();
    testIntegra.m_integra32.armAlarm();

    workStubSatel = false;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    testIntegra.checkSatel();
    testIntegra.m_integra32.armAlarm();

    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    testIntegra.checkSatel();

}
