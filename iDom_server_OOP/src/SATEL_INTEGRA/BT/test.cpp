#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>

#include "../satel_integra_handler.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

// armed partition fefe0a000000007dc4fe0d
// output states fefe1700000000000000000000000000000000430efe0d


std::atomic<bool> workStubSatel = true;
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

                    std::cout << " STUB SATEL RECV: " << m_recv_size  << std::endl;
                    if(c_buffer[2] == INTEGRA_ENUM::VERSION){
                        std::cout << "wersja integry" << std::endl;
                        std::string len;
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x41);
                        len.push_back(0xae);
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
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x43);
                        len.push_back(0x0e);
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


                        int siz = send(v_sock_ind, c_buffer, m_recv_size, 0);
                        if( siz >= 0 )
                        {
                            std::cout << "wyslano "<< std::endl;
                        }
                        else
                            std::cout << " nie wyslano" << std::endl;
                    }
                    else if(c_buffer[2] == (char)INTEGRA_ENUM::DISARM){
                        std::cout << "rozbrojenie alarmu "  << (int)c_buffer[3] << " " << (int)c_buffer[4] << std::endl;
                        std::string len;


                        int siz = send(v_sock_ind, c_buffer, m_recv_size, 0);
                        if( siz >= 0 )
                        {
                            std::cout << "wyslano "<< std::endl;
                        }
                        else
                            std::cout << " nie wyslano" << std::endl;
                    }
                    else if(c_buffer[2] == (char)INTEGRA_ENUM::OUTPUT_ON){
                        std::cout << "zmiana stanu alarmu integry" << std::endl;
                        std::string len;
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::OUTPUT_ON);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x85);
                        len.push_back(0xc4);

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
                    else if(c_buffer[2] == (char)INTEGRA_ENUM::OUTPUT_OFF){
                        std::cout << "zmiana stanu alarmu integry" << std::endl;
                        std::string len;
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::HEADER_MSG);
                        len.push_back(INTEGRA_ENUM::OUTPUT_OFF);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x85);
                        len.push_back(0xd4);

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
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x00);
                        len.push_back(0x7d);
                        len.push_back(0xc4);

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
                    else{
                        std::cout << "SATEL STUB SERVER nieznany przypadek :( " << std::hex << (int)c_buffer[2] << std::endl;
                        assert(false);
                    }
                }
                else{
                    std::cout << "odebrano tylko : " <<m_recv_size << std::endl;
                }
            }
        }

    } // while
    std::cout <<  "zamykamy stub serwera integra" << std::endl;
    close(v_socket);
    workStubSatel = true;
}


class satel_integra_fixture: public iDomTOOLS_ClassTest
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
        iDomTOOLS_ClassTest::SetUp();
    }
    void TearDown()
    {
        workStubSatel = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        iDomTOOLS_ClassTest::TearDown();
        std::cout << "satel_integra_fixture TearDown()" << std::endl;
    }

};

TEST_F(satel_integra_fixture, checkIntegraOut)
{
    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    SATEL_INTEGRA_HANDLER testIntegra(&test_my_data);
    testIntegra.checkSatel();
    dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->getIntegraInfo();
    EXPECT_FALSE(testIntegra.getSatelPTR()->isAlarmArmed());
}

TEST_F(satel_integra_fixture, main)
{
    //TODO
    return ;
    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    SATEL_INTEGRA_HANDLER testIntegra(&test_my_data);
    testIntegra.checkSatel();
    testIntegra.getSatelPTR()->armAlarm(1);

    workStubSatel = false; //close satel server stub

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    while(workStubSatel == false){
     puts("kkkk");
    }
    //testIntegra.checkSatel();
    std::cout << "DUPAAA " << workStubSatel << std::endl;
    EXPECT_ANY_THROW( testIntegra.getSatelPTR()->armAlarm(1));

    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    testIntegra.checkSatel();
}

TEST_F(satel_integra_fixture, turnOnOffOutput)
{
    startSatelServer();
    SATEL_INTEGRA_HANDLER testIntegra(&test_my_data);

    //   std::cout << testIntegra.getSatelPTR()->getIntegraInfo() << std::endl;
    //testIntegra.checkSatel();

    testIntegra.getSatelPTR()->outputOn(3);
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    testIntegra.checkSatel();
    testIntegra.getSatelPTR()->outputOff(3);
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    testIntegra.checkSatel();
}

TEST_F(satel_integra_fixture, isArmed)
{
    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    SATEL_INTEGRA_HANDLER testIntegra(&test_my_data);

    EXPECT_FALSE(testIntegra.getSatelPTR()->isAlarmArmed() );
}

TEST_F(satel_integra_fixture, armAndDisarm)
{
    unsigned int partitionID = 1;
    startSatelServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    SATEL_INTEGRA_HANDLER testIntegra(&test_my_data);
    testIntegra.getSatelPTR()->armAlarm(partitionID);

    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[0], INTEGRA_ENUM::HEADER_MSG);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[1], INTEGRA_ENUM::HEADER_MSG);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[2], INTEGRA_ENUM::ARM);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[3], 0x11);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[4], 0x22);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[5], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[6], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[7], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[8], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[9], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[10], 0xff);
    // partition
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[11], 0x01);

    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[12], 0x00);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[13], 0x00);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[14], 0x00);

    testIntegra.getSatelPTR()->disarmAlarm(partitionID);

    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[0], INTEGRA_ENUM::HEADER_MSG);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[1], INTEGRA_ENUM::HEADER_MSG);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[2], INTEGRA_ENUM::DISARM);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[3], 0x11);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[4], 0x22);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[5], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[6], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[7], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[8], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[9], 0xff);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[10], 0xff);
    // partition
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[11], 0x01);

    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[12], 0x00);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[13], 0x00);
    EXPECT_EQ(dynamic_cast<SATEL_INTEGRA*>(testIntegra.getSatelPTR())->m_message[14], 0x00);

}


