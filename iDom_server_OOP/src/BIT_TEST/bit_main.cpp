#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>

#include "../iDomTools/test/iDomTools_fixture.h"
#include "../TASKER/tasker.h"
#include "../src/thread_functions/iDom_thread.h"
#include "../src/blockQueue/blockqueue.h"
#include "../src/recuperator/recuperator.h"

class bit_fixture : public iDomTOOLS_ClassTest
{
protected:
    struct sockaddr_in server;
    int v_socket;
    CONFIG_JSON testCS;
    const char *ipAddress = "127.0.0.1";
    void start_iDomServer();
    void crypto(std::string &toEncrypt, std::string &key, bool encrypted);

    std::unique_ptr<TASKER> bit_Tasker;
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> thread_array;
    std::string send_receive(int socket, std::string msg, std::string key, bool crypt = true);

    bit_fixture() : v_socket(0),
                    bit_Tasker(std::make_unique<TASKER>(&test_context))

    {
        test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("cyniu-BIT");
        test_context.ptr_buderus = std::make_unique<BUDERUS>();
        for (size_t i = 0; i < thread_array.size(); ++i)
        {
            thread_array[i].thread_name = "  -empty-  ";
            thread_array[i].thread_socket = 0;
        }
        test_context.ptr_MPD_info = std::make_unique<MPD_info>();
        test_context.main_THREAD_arr = &thread_array;
        test_context.server_settings = &testCS;
        test_context.server_settings->_server.encrypted = true;
        test_context.server_settings->_fb_viber.viberReceiver = {"test1", "test2};"};
        test_context.server_settings->_fb_viber.viberSender = "testViberSender";

        test_context.main_iDomStatus = std::make_unique<iDomSTATUS>();
        test_context.main_REC = std::make_shared<RADIO_EQ_CONTAINER>(&test_context);
        test_context.main_iDomTools = std::make_shared<iDomTOOLS>(&test_context);
        test_context.server_settings->_recuperation.MQTT_SENSOR_TOPIC = "iDom-client/comfoconnect/sensor";
        test_context.server_settings->_recuperation.MQTT_CONTROL_TOPIC = "iDom-client/comfoconnect/control";
        test_context.m_recuperator = std::make_unique<Recuperator>(&test_context);
        test_context.server_settings->_server.PORT = 8833;
        test_context.server_settings->_server.SERVER_IP = "127.0.0.1";
        test_context.server_settings->_runThread.SATEL = true;
        test_context.server_settings->_command = nlohmann::json::parse(R"({
        "lock":["jedna komenda", "druga komenda"],
        "unlock":["jedna komenda unlock", "druga komenda unlock"],
        "sunrise":{
            "lock": ["sjedna komenda sunrise lock", "sdruga komenda sunrise lock"],
            "unlock": ["sjedna komenda sunrise unlock", "sdruga komenda sunrise unlock"]
        },
        "sunset":{
            "unlock": ["sjedna komenda sunset unlock", "sdruga komenda sunset unlock"],
            "lock": ["sjedna komenda sunset lock", "sdruga komenda sunset lock"]
        }
    })");
    }
};

void bit_fixture::crypto(std::string &toEncrypt, std::string &key, bool encrypted)
{
    if (!encrypted)
    {
        return;
    }
    unsigned int keySize = key.size() - 1;

    for (char &i : toEncrypt)
    {
        if (keySize == 0)
            keySize = key.size() - 1;
        else
            --keySize;
        i ^= key[keySize];
    }
}
void bit_fixture::start_iDomServer()
{
    useful_F::workServer = true; // włącz nasluchwianie servera
    useful_F::go_while = true;
    auto t = std::thread(&useful_F::startServer, &test_context, bit_Tasker.get());
    t.detach();
    std::cout << "EXIT bit_fixture::start_iDomServer()" << std::endl;
}

std::string bit_fixture::send_receive(int socket, std::string msg, std::string key, bool crypt)
{
    char buffer_new[10000];
    std::string ret;
    std::string ok = "ok";

    std::cout << " wysłałem: " << msg << std::endl;
    crypto(msg, key, crypt);
    send(socket, msg.c_str(), msg.size(), 0);
    ssize_t size = recv(socket, buffer_new, sizeof(buffer_new), 0);
    for (ssize_t i = 0; i < size; ++i)
    {
        ret.push_back(buffer_new[i]);
    }
    crypto(ret, key, crypt);
    std::cout << " w połowie " << ret << std::endl;
    ssize_t sizeRec = static_cast<ssize_t>(std::stoul(ret));
    ret.clear();

    crypto(ok, key, crypt);
    send(socket, ok.c_str(), ok.size(), 0);

    size = recv(socket, buffer_new, sizeof(buffer_new), 0);

    while (size not_eq sizeRec)
    {
        std::cout << " w while: " << size << " recSize: " << sizeRec << std::endl;
        size += recv(socket, buffer_new, sizeof(buffer_new), 0);
    }

    EXPECT_EQ(size, sizeRec);
    for (ssize_t i = 0; i < size; ++i)
    {
        ret.push_back(buffer_new[i]);
    }
    crypto(ret, key, crypt);
    std::cout << " kończymy z " << ret << std::endl;
    return ret;
}

TEST_F(bit_fixture, socket_heandle_command)
{
    start_iDomServer();
    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);
    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);
    sleep(1);

    int connectStatus = connect(s, reinterpret_cast<struct sockaddr *>(&serwer), sizeof(serwer));

    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key, key);
    toCheck = send_receive(s, "ROOT", key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;
    std::cout << "odebrano5: " << send_receive(s, "help", key) << std::endl;

    toCheck = send_receive(s, "exit", key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr("END"));

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR);

    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);

    toCheck = test_context.iDomAlarm.showAlarm();
    EXPECT_STREQ(toCheck.c_str(), "88756: 433MHz equipment not found first\n");
}

TEST_F(bit_fixture, socket_heandle_command_lock)
{
    start_iDomServer();
    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);
    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);
    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key, key);
    toCheck = send_receive(s, "ROOT", key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;
    std::cout << "odebrano5: " << send_receive(s, "iDom lock", key) << std::endl;

    toCheck = send_receive(s, "exit", key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr("END"));

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR);

    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);

    toCheck = test_context.iDomAlarm.showAlarm();
    EXPECT_STREQ(toCheck.c_str(), "88756: 433MHz equipment not found first\n");
}

TEST_F(bit_fixture, socket_close_server_command)
{

    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key, key);
    toCheck = send_receive(s, "ROOT", key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;

    EXPECT_TRUE(useful_F::workServer);
    toCheck = send_receive(s, "program stop", key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr("CLOSE"));

    close(s);
    shutdown(s, SHUT_RDWR);
    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
    EXPECT_FALSE(useful_F::workServer);
    EXPECT_EQ(test_context.iDomProgramState, iDomStateEnum::CLOSE);
}

TEST_F(bit_fixture, socket_reload_server_command)
{

    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key, key);
    toCheck = send_receive(s, "ROOT", key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;

    EXPECT_TRUE(useful_F::workServer);
    toCheck = send_receive(s, "program reload hard", key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr("CLOSE"));

    close(s);
    shutdown(s, SHUT_RDWR);
    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
    EXPECT_FALSE(useful_F::workServer);
    EXPECT_EQ(test_context.iDomProgramState, iDomStateEnum::HARD_RELOAD);
}

TEST_F(bit_fixture, socket_reboot_rasp_command)
{

    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key, key);
    toCheck = send_receive(s, "ROOT", key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;

    EXPECT_TRUE(useful_F::workServer);
    toCheck = send_receive(s, "program raspberry reboot", key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr("command done with exitcode: 0"));

    close(s);
    shutdown(s, SHUT_RDWR);
    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
    EXPECT_FALSE(useful_F::workServer);
}

TEST_F(bit_fixture, socket_wrong_key_after_while)
{

    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    send_receive(s, key, key);
    toCheck = send_receive(s, "ROOT", key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;
    std::cout << "odebrano5: " << send_receive(s, "help", key) << std::endl;

    EXPECT_ANY_THROW(send_receive(s, "test", "fake"));

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR);

    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
}

TEST_F(bit_fixture, socket_no_space_left_on_server)
{

    for (auto &i : *test_context.main_THREAD_arr)
    {
        i.thread_socket = 1;
    }
    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string toCheck;

    EXPECT_ANY_THROW(send_receive(s, key, key));
    ////////////////////////////////////////////////// one free slot
    test_context.main_THREAD_arr->at(3).thread_socket = 0;
    const int s2 = socket(serwer.sin_family, SOCK_STREAM, 0);
    connectStatus = connect(s2, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    EXPECT_NO_THROW(send_receive(s2, key, key));

    toCheck = send_receive(s2, "ROOT", key);
    EXPECT_STREQ(toCheck.c_str(), "OK you are ROOT");

    std::cout << "odebrano4: " << toCheck << std::endl;
    std::cout << "odebrano5: " << send_receive(s2, "help", key) << std::endl;

    toCheck = send_receive(s2, "exit", key);

    std::cout << "odebrano8: " << toCheck << std::endl;
    EXPECT_THAT(toCheck.c_str(), testing::HasSubstr("END"));

    useful_F::workServer = false;
    close(s);
    close(s2);
    shutdown(s, SHUT_RDWR);
    shutdown(s2, SHUT_RDWR);
    for (auto &i : *test_context.main_THREAD_arr)
    {
        i.thread_socket = 0;
    }
    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
}

TEST_F(bit_fixture, socket_send_key_fast_disconnect)
{
    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    std::string key = useful_F::RSHash();
    std::string msgKey(key);
    crypto(msgKey, key, true);
    int r = send(s, msgKey.c_str(), msgKey.size(), 0);

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR);

    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
    EXPECT_EQ(r, key.size());
}

TEST_F(bit_fixture, socket_connection_wrong_key_fast_disconnect)
{
    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    std::string fakeKey = "fake";
    int r = send(s, fakeKey.c_str(), fakeKey.size(), 0);
    EXPECT_EQ(r, 4);
    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR);

    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
}

TEST_F(bit_fixture, socket_connection_wrong_key)
{
    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    auto key = useful_F::RSHash();
    std::string fakeKey = key + "fake";
    std::string toCheck = send_receive(s, fakeKey, key);
    std::cout << "odebrano: " << toCheck << std::endl;
    EXPECT_STREQ(toCheck.c_str(), "\nFAIL\n");

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR);

    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
}

TEST_F(bit_fixture, socket_connection_http)
{
    start_iDomServer();

    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(8833)};

    inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

    const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

    sleep(1);
    int connectStatus = connect(s, (struct sockaddr *)&serwer, sizeof(serwer));
    ASSERT_EQ(connectStatus, 0);
    std::cout << "connect status: " << connectStatus << std::endl;

    std::string httpMsg = R"(
 POST /dupa HTTP/1.1
Host: cyniu88.no-ip.pl:8833
User-Agent: ESP8266HTTPClient
Accept-Encoding: identity;q=1,chunked;q=0.1,*;q=0
Connection: keep-alive
Content-Type: iDom/logging
Content-Length: 43

{"msg":"start Rolet Rydzyka","millis":9899}

)";
    send(s, httpMsg.c_str(), httpMsg.size(), 0);

    std::this_thread::sleep_for(100ms);
    char buffer_new[10000];
    auto recC = recv(s, buffer_new, sizeof(buffer_new), 0);
    std::cout << "!!! odebrano: size " << recC << "###" << std::endl;

    std::string toCheck;
    for (int i = 0; i < recC; ++i)
    {
        toCheck.push_back(buffer_new[i]);
    }
    std::cout << "odebrano: " << toCheck << std::endl;
    EXPECT_THAT(toCheck, ::testing::HasSubstr("HTTP/1.1 200 OK"));

    close(s);

    useful_F::workServer = false;
    shutdown(s, SHUT_RDWR);

    iDOM_THREAD::waitUntilAllThreadEnd(&test_context);
}

TEST_F(bit_fixture, buderus_mqtt_command_from_boiler)
{

    EXPECT_FALSE(test_context.ptr_buderus->isHeatingActiv());

    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active", "1");
    bit_Tasker->runTasker();
    EXPECT_TRUE(test_context.ptr_buderus->isHeatingActiv());

    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active", "0");
    bit_Tasker->runTasker();
    EXPECT_FALSE(test_context.ptr_buderus->isHeatingActiv());

    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active", "1");
    bit_Tasker->runTasker();
    EXPECT_TRUE(test_context.ptr_buderus->isHeatingActiv());

    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/heating_active", "offline");
    bit_Tasker->runTasker();
    EXPECT_FALSE(test_context.ptr_buderus->isHeatingActiv());
    /////////////////////////////////  boiler data //////////////////////////////////////////////////////
    std::string test_boilerData = R"({"heatingActive":"off","tapwaterActive":"off","selFlowTemp":5,"selBurnPow":0,"curBurnPow":0,"heatingPumpMod":0,"outdoorTemp":11.9,"curFlowTemp":23.5,"burnGas":"off","flameCurr":0,"heatingPump":"off","fanWork":"off","ignWork":"off","heatingActivated":"on","heatingTemp":35,"pumpModMax":100,"pumpModMin":10,"pumpDelay":3,"burnMinPeriod":10,"burnMinPower":0,"burnMaxPower":71,"boilHystOn":-6,"boilHystOff":6,"burnStarts":3721,"burnWorkMin":372217,"heatWorkMin":337732,"UBAuptime":1683134,"serviceCode":"0H","serviceCodeNumber":203,"lastCode":"6L(229) 23.12.2021 20:24","maintenanceMessage":"-","maintenance":"off"})";

    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/boiler_data", test_boilerData);
    bit_Tasker->runTasker();
    auto ret = test_context.ptr_buderus->getDump();
    std::cout << ret << std::endl;
    EXPECT_EQ(test_context.ptr_buderus->getOutdoorTemp(), 11.9);
    EXPECT_THAT(ret, ::testing::HasSubstr("1683134"));

    /////////////////////////////////  boiler data ww //////////////////////////////////////////////////////
    std::string test_boilerData_ww = R"({"wWComfort":"Eco","wWSelTemp":60,"wWSetTemp":10,"wWDisinfectionTemp":70,"wWType":"buffer","wWChargeType":"3-way valve","wWCircPump":"on","wWCircPumpMode":"continuous","wWCirc":"off","wWCurTemp":64,"wWCurTemp2":64,"wWCurFlow":0,"wWStorageTemp2":64,"wWActivated":"on","wWOneTime":"off","wWDisinfecting":"off","wWCharging":"off","wWRecharging":"off","wWTempOK":"on","wWActive":"off","wWHeat":"off","wWStarts":1762,"wWMaxPower":100,"wWWorkM":34502})";

    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/boiler_data_ww", test_boilerData_ww);
    bit_Tasker->runTasker();
    ret = test_context.ptr_buderus->getDump();
    std::cout << ret << std::endl;
    EXPECT_EQ(test_context.ptr_buderus->getBoilerTemp(), 64);

    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/thermostat_data",
                                                R"({"some":"data"})");
    bit_Tasker->runTasker();
    ret = test_context.ptr_buderus->getDump();
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, ::testing::HasSubstr(R"(some":)"));
}

TEST_F(bit_fixture, buderus_mqtt_command_from_boiler_wrong_json_format)
{
    /////////////////////////////////  boiler data //////////////////////////////////////////////////////
    std::string test_boilerData = "not json";
    test_context.mqttHandler->putToReceiveQueue("iDom-client/buderus/ems-esp/boiler_data", test_boilerData);
    bit_Tasker->runTasker();
    auto ret = test_context.ptr_buderus->getDump();
    std::cout << ret << std::endl;
    ret = test_context.iDomAlarm.showAlarm();
    EXPECT_THAT(ret, ::testing::HasSubstr("buderus boile_data - wrong JSON format!"));
}

TEST_F(bit_fixture, tasker_no_action)
{
    EXPECT_EQ(256, bit_Tasker->runTasker());
}

TEST_F(bit_fixture, mqtt_command)
{
    blockQueue testMPD_Q;
    testMPD_Q._clearAll();
    test_context.mqttHandler->putToReceiveQueue("iDom-client/command", "MPD volume up");
    bit_Tasker->runTasker();
    EXPECT_EQ(1, testMPD_Q._size());
    EXPECT_EQ(testMPD_Q._get(), MPD_COMMAND::VOLUP);
}

TEST_F(bit_fixture, mqtt_command_shed)
{
    test_context.lusina.shedConfJson = nlohmann::json::parse(R"({
    "deepSleep":true,
    "howLongDeepSleep":177,
    "fanON":false})");
    test_context.mqttHandler->putToReceiveQueue("shed/put", "MPD volume up");
    bit_Tasker->runTasker();
    test_context.mqttHandler->putToReceiveQueue("shed/put", R"({"temperatura":20.85000038,"ciśnienie":971.899231,"wilgotność":53.17382813,"millis":15273,"bateria":3.896484375,"log":"Found BME280 sensor! Couldn't find PCF8574","fanON":false})");
    bit_Tasker->runTasker();
}

TEST_F(bit_fixture, start_iDom_unlock_lock)
{
    EXPECT_FALSE(test_context.satelIntegraHandler->getSatelPTR()->isAlarmArmed());
    test_context.main_iDomTools->unlockHome();
    EXPECT_FALSE(test_context.satelIntegraHandler->getSatelPTR()->isAlarmArmed());
    test_context.main_iDomTools->lockHome();
    EXPECT_TRUE(test_context.satelIntegraHandler->getSatelPTR()->isAlarmArmed());
    test_context.main_iDomTools->unlockHome();
    EXPECT_FALSE(test_context.satelIntegraHandler->getSatelPTR()->isAlarmArmed());
}

TEST_F(bit_fixture, recuperation_base)
{
    test_context.mqttHandler->putToReceiveQueue("iDom-client/comfoconnect/sensor/wilgotnosc", "45");
    bit_Tasker->runTasker();
    test_context.mqttHandler->putToReceiveQueue("iDom-client/comfoconnect/sensor/wilgotnosc", "46");
    bit_Tasker->runTasker();
    test_context.mqttHandler->putToReceiveQueue("iDom-client/comfoconnect/sensor/wilgotnosc2", "46");
    bit_Tasker->runTasker();
    std::cout << "event: " << test_context.myEventHandler.run("recuperation")->getEvent() << std::endl;

    auto resultData = test_context.m_recuperator->getData();

    EXPECT_EQ(std::any_cast<float>(resultData["recuperator"]["wilgotnosc"].value()), 45.5);
    EXPECT_EQ(resultData["recuperator"].size(), 2);

    test_context.mqttHandler->putToReceiveQueue("iDom-client/comfoconnect/sensor/wilgotnosc", "25");
    bit_Tasker->runTasker();
    test_context.mqttHandler->putToReceiveQueue("iDom-client/comfoconnect/sensor/wilgotnosc", "75");
    bit_Tasker->runTasker();

    resultData = test_context.m_recuperator->getData();

    EXPECT_NEAR(std::any_cast<float>(resultData["recuperator"]["wilgotnosc"].value()), 48.666698, 1e-4);
    EXPECT_EQ(resultData["recuperator"].size(), 2);

    auto dane = test_context.m_recuperator->getData();
    std::cout << "wilgotnosc: " << std::any_cast<float>(dane["recuperator"]["wilgotnosc"].value()) << std::endl;
    std::cout << "wilgotnosc2: " << std::any_cast<float>(dane["recuperator"]["wilgotnosc2"].value()) << std::endl;
}
