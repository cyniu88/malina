#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>

#include "../iDomTools/test/iDomTools_fixture.h"
#include "../TASKER/tasker.h"
#include "../src/thread_functions/iDom_thread.h"

class bit_fixture : public iDomTOOLS_ClassTest
{
protected:
    struct sockaddr_in server;
    int v_socket;
    CONFIG_JSON testCS;
    CAMERA_CFG testCamera;
    const char *ipAddress = "127.0.0.1";

    void SetUp()
    {
        iDomTOOLS_ClassTest::SetUp();
        bit_Tasker = std::make_unique<TASKER>(&test_my_data);
        test_my_data.mqttHandler = std::make_unique<MQTT_mosquitto>("cyniu-BIT");
        test_my_data.ptr_buderus = std::make_unique<BUDERUS>();
        for (size_t i = 0; i < thread_array.size(); ++i)
        {
            thread_array[i].thread_name = "  -empty-  ";
            thread_array[i].thread_socket = 0;
        }
        test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();
        test_my_data.main_THREAD_arr = &thread_array;
        test_my_data.server_settings = &testCS;
        test_my_data.server_settings->_server.encrypted = true;
        test_my_data.server_settings->_camera = testCamera;
        test_my_data.server_settings->_camera.cameraLedOFF = " not set";
        test_my_data.server_settings->_fb_viber.viberReceiver = {"test1","test2};"};
        test_my_data.server_settings->_fb_viber.viberSender = "testViberSender";

        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
        test_my_data.main_REC = std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data);
        test_my_data.main_iDomTools = std::make_unique<iDomTOOLS>(&test_my_data);
        test_my_data.server_settings->_server.PORT = 8833;
        test_my_data.server_settings->_server.SERVER_IP = "127.0.0.1";
    }
    void TearDown()
    {
        iDomTOOLS_ClassTest::TearDown();
    }
public:
    void start_iDomServer();
    void crypto(std::string & toEncrypt, std::string &key, bool encrypted);

    std::unique_ptr<TASKER> bit_Tasker;
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> thread_array;
    std::string send_receive(int socket, std::string msg, std::string key, bool crypt = true);
};

void bit_fixture::crypto (std::string & toEncrypt, std::string& key,bool encrypted)
{
    if (!encrypted){
        return;
    }
    unsigned int keySize = key.size() - 1;

    for (char & i : toEncrypt)
    {
        if (keySize == 0) keySize = key.size()-1;
        else --keySize;
        i ^= key[keySize];
    }
}
void bit_fixture::start_iDomServer()
{
    useful_F::workServer = true; // włącz nasluchwianie servera
    useful_F::go_while = true;
    auto t = std::thread(&useful_F::startServer, &test_my_data,bit_Tasker.get());
    t.detach();
    std::cout << "EXIT bit_fixture::start_iDomServer()" << std::endl;
}

std::string bit_fixture::send_receive(int socket, std::string msg, std::string key,bool crypt)
{
    char buffer[10000];
    std::string ret;
    std::string ok = "ok";

    std::cout << " wysłałem: " << msg << std::endl;
    crypto(msg,key, crypt);
    send( socket, msg.c_str(), msg.size(), 0 );
    ssize_t size = recv( socket, buffer, sizeof( buffer ), 0 );
    for (ssize_t i = 0 ; i < size; ++i){
        ret.push_back(buffer[i]);
    }
    crypto(ret,key, crypt);
    std::cout << " w połowie " << ret << std::endl;
    ssize_t sizeRec = static_cast<ssize_t>(std::stoul(ret));
    ret.clear();

    crypto(ok,key, crypt);
    send( socket, ok.c_str(), ok.size(), 0 );

    size = recv( socket, buffer, sizeof( buffer ), 0 );

    while(size != sizeRec){
        std::cout << " w while: " << size << " recSize: " << sizeRec << std::endl;
        size += recv( socket, buffer, sizeof( buffer ), 0 );
    }

    EXPECT_EQ(size, sizeRec);
    for (ssize_t i = 0 ; i < size; ++i){
        ret.push_back(buffer[i]);
    }
    crypto(ret,key, crypt);
    std::cout << " kończymy z " << ret<< std::endl;
    return ret;
}

TEST_F(bit_fixture, socket_heandle_command){
    start_iDomServer();
    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
                .sin_port = htons( 8833 )
    };

    inet_pton( serwer.sin_family, ipAddress, & serwer.sin_addr );
    const int s = socket( serwer.sin_family, SOCK_STREAM, 0 );
    sleep(1);
    int connectStatus = connect(s,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key,key);
    toCheck = send_receive(s, "ROOT",key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;
    std::cout << "odebrano5: " << send_receive(s, "help",key) << std::endl;

    toCheck = send_receive(s, "exit", key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr( "END"));

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR );

    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);

    toCheck = test_my_data.iDomAlarm.showAlarm();
    EXPECT_STREQ(toCheck.c_str(), "88756: 433MHz equipment not found first\n");
}

TEST_F(bit_fixture, socket_close_server_command){

    start_iDomServer();

    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
                .sin_port = htons( 8833 )
    };

    inet_pton( serwer.sin_family, ipAddress, & serwer.sin_addr );

    const int s = socket( serwer.sin_family, SOCK_STREAM, 0 );

    sleep(1);
    int connectStatus = connect(s,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key,key);
    toCheck = send_receive(s, "ROOT",key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;

    EXPECT_TRUE(useful_F::workServer);
    toCheck = send_receive(s, "program stop",key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr( "CLOSE"));

    close(s);
    shutdown(s, SHUT_RDWR );
    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);
    EXPECT_FALSE(useful_F::workServer);
}
TEST_F(bit_fixture, socket_wrong_key_after_while){

    start_iDomServer();

    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
                .sin_port = htons( 8833 )
    };

    inet_pton( serwer.sin_family, ipAddress, & serwer.sin_addr );

    const int s = socket( serwer.sin_family, SOCK_STREAM, 0 );

    sleep(1);
    int connectStatus = connect(s,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key,key);
    toCheck = send_receive(s, "ROOT",key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;
    std::cout << "odebrano5: " << send_receive(s, "help",key) << std::endl;

    EXPECT_ANY_THROW(send_receive(s, "test","fake"));

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR );

    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);
}
TEST_F(bit_fixture, socket_no_space_left_on_server){

    for(auto& i : *test_my_data.main_THREAD_arr){
        i.thread_socket = 1;
    }
    start_iDomServer();

    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
                .sin_port = htons( 8833 )
    };

    inet_pton( serwer.sin_family, ipAddress, & serwer.sin_addr );

    const int s = socket( serwer.sin_family, SOCK_STREAM, 0 );

    sleep(1);
    int connectStatus = connect(s,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    EXPECT_ANY_THROW(send_receive(s, key,key));
    ////////////////////////////////////////////////// one free slot
    test_my_data.main_THREAD_arr->at(3).thread_socket = 0;
    const int s2 = socket( serwer.sin_family, SOCK_STREAM, 0 );
    connectStatus = connect(s2,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    EXPECT_NO_THROW(send_receive(s2, key,key));

    toCheck = send_receive(s2, "ROOT",key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;
    std::cout << "odebrano5: " << send_receive(s2, "help",key) << std::endl;

    toCheck = send_receive(s2, "exit",key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr( "END"));

    useful_F::workServer = false;
    close(s);
    close(s2);
    shutdown(s, SHUT_RDWR );
    shutdown(s2, SHUT_RDWR );
    for(auto& i : *test_my_data.main_THREAD_arr){
        i.thread_socket = 0;
    }
    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);
}
TEST_F(bit_fixture, socket_send_key_fast_disconnect){
    start_iDomServer();

    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
                .sin_port = htons( 8833 )
    };

    inet_pton( serwer.sin_family, ipAddress, & serwer.sin_addr );

    const int s = socket( serwer.sin_family, SOCK_STREAM, 0 );

    sleep(1);
    int connectStatus = connect(s,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    std::string key = useful_F::RSHash();
    std::string msgKey(key);
    crypto(msgKey,key,true);
    int r = send( s, msgKey.c_str(), msgKey.size(), 0 );

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR );

    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);
    EXPECT_EQ(r, key.size());
}
TEST_F(bit_fixture, socket_connection_wrong_key_fast_disconnect){
    start_iDomServer();

    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
                .sin_port = htons( 8833 )
    };

    inet_pton( serwer.sin_family, ipAddress, & serwer.sin_addr );

    const int s = socket( serwer.sin_family, SOCK_STREAM, 0 );

    sleep(1);
    int connectStatus = connect(s,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    std::string fakeKey = "fake";
    int r = send( s, fakeKey.c_str(), fakeKey.size(), 0 );
    EXPECT_EQ(r, 4);
    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR );

    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);
}

TEST_F(bit_fixture, socket_connection_wrong_key){
    start_iDomServer();

    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
                .sin_port = htons( 8833 )
    };

    inet_pton( serwer.sin_family, ipAddress, & serwer.sin_addr );

    const int s = socket( serwer.sin_family, SOCK_STREAM, 0 );

    sleep(1);
    int connectStatus = connect(s,( struct sockaddr * ) & serwer, sizeof( serwer ) );
    ASSERT_EQ(connectStatus,0);
    std::cout << "connect status: "<< connectStatus <<std::endl;

    auto key = useful_F::RSHash();
    std::string fakeKey = key + "fake";
    std::string toCheck = send_receive(s, fakeKey,key);
    std::cout << "odebrano: " << toCheck << std::endl;
    EXPECT_STREQ(toCheck.c_str(), "\nFAIL\n");

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR );

    iDOM_THREAD::waitUntilAllThreadEnd(&test_my_data);
}

TEST_F(bit_fixture, buderus_mqtt_command_from_boiler){

    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","1");
    bit_Tasker->runTasker();
    EXPECT_TRUE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","0");
    bit_Tasker->runTasker();
    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","1");
    bit_Tasker->runTasker();
    EXPECT_TRUE(test_my_data.ptr_buderus->isHeatingActiv());

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active","offline");
    bit_Tasker->runTasker();
    EXPECT_FALSE(test_my_data.ptr_buderus->isHeatingActiv());
    /////////////////////////////////  boiler data //////////////////////////////////////////////////////
    std::string test_boilerData = R"({"wWComfort":"Hot",   "wWSelTemp":60,   "wWDesiredTemp":70,   "selFlowTemp":5,   "selBurnPow":0,   "curBurnPow":0,   "pumpMod":10,   "wWCircPump":0,   "curFlowTemp":30.9,   "switchTemp":0,)"
                                  R"("boilTemp":16.4,   "wWActivated":"off",   "wWOnetime":"on",   "burnGas":"on",   "flameCurr":0,   "heatPmp":"on",   "fanWork":"off",   "ignWork":"off",   "wWCirc":"off",)"
                                  R"("heating_temp":50,   "outdoorTemp":9.99,   "wwStorageTemp2":62.2,   "pump_mod_max":100,   "pump_mod_min":10,   "wWHeat":"off",   "UBAuptime":14590,   "burnStarts":27,   "burnWorkMin":13594,   "heatWorkMin":13594,   "ServiceCode":"0H",)"
                                  R"("ServiceCodeNumber":203})";

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/boiler_data",test_boilerData);
    bit_Tasker->runTasker();
    auto ret = test_my_data.ptr_buderus->dump();
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr("13594"));

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/thermostat_data",
                                                R"({"some":"data"})");
    bit_Tasker->runTasker();
    ret = test_my_data.ptr_buderus->dump();
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr(R"(some":)"));
}

TEST_F(bit_fixture, buderus_mqtt_command_from_boiler_wrong_json_format){
    /////////////////////////////////  boiler data //////////////////////////////////////////////////////
    std::string test_boilerData = "not json";
    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/boiler_data",test_boilerData);
    bit_Tasker->runTasker();
    auto ret = test_my_data.ptr_buderus->dump();
    std::cout << ret << std::endl;
    ret = test_my_data.iDomAlarm.showAlarm();
    EXPECT_THAT(ret, ::testing::HasSubstr("buderus boile_data - wrong JSON format!"));
}

TEST_F(bit_fixture, tasker_lusina){
    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/command/lusina/t", "11");
    bit_Tasker->runTasker();
    EXPECT_STREQ(test_my_data.lusina.temperatureDS20.c_str() , "11");

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/command/lusina/t", "112");
    bit_Tasker->runTasker();
    EXPECT_STREQ(test_my_data.lusina.temperatureDS20.c_str() , "112");

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/command/lusina/h","wilgotnosc 22 temepratura 33");
    bit_Tasker->runTasker();
    EXPECT_STREQ(test_my_data.lusina.humidityDTH.c_str() , "22");

    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/command/lusina/h","wilgotnosc 33 temepratura 33");
    bit_Tasker->runTasker();
    EXPECT_STREQ(test_my_data.lusina.humidityDTH.c_str() , "33");
    //////////////////////// now fake data
    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/command/lusina/h","wilgotnosc brak temepratura 33");
    bit_Tasker->runTasker();
    EXPECT_EQ(test_my_data.lusina.statHumi.min() , -1);
}

TEST_F(bit_fixture, tasker_no_action){
    EXPECT_EQ(256, bit_Tasker->runTasker());
}

TEST_F(bit_fixture, mqtt_command){
    blockQueue testMPD_Q;
    testMPD_Q._clearAll();
    test_my_data.mqttHandler->putToReceiveQueue("iDom-client/command","MPD volume up");
    bit_Tasker->runTasker();
    EXPECT_EQ(1, testMPD_Q._size());
    EXPECT_EQ(testMPD_Q._get(), MPD_COMMAND::VOLUP);
}
