#include "iDom_server_OOP.h"
#include "functions/functions.h"
#include <wiringPi.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "CRON/cron.hpp"
#include "functions/functions.h"
#include "functions/mpd_cli.h"
#include "TASKER/tasker.h"
#include "RADIO_433_eq//radio_433_eq.h"
#include "thread_functions/rs232_thread.h"
#include "433MHz/RFLink/rflinkhandler.h"
#include "command/commandClass/command_ardu.h"
#include "thread_functions/iDom_thread.h"
#include "MENU/menu_root.h"
#include "SATEL_INTEGRA/satel_integra_handler.h"
#include "buderus/buderus.h"
#include "iDomTools/idomtools.h"
#include "iDomSaveState/idom_save_state.h"
#include "../libs/backTrace/backTrace.h"
#include "dbClient/db-client-factory.hpp"

using namespace std::chrono_literals;

std::string _logfile = "/mnt/ramdisk/iDom_log.log";
std::string buffer;
Logger log_file_mutex(_logfile);

//////////////// watek RFLink //////////////////////////////////
void RFLinkHandlerRUN(thread_data *context, const std::string &threadName)
{
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "watek " << threadName << " wystartowal " << std::this_thread::get_id() << std::endl;
    log_file_mutex.mutex_unlock();
    std::string msgFromRFLink;
    RC_433MHz rc433(context);

    context->main_RFLink->flush();
    std::vector<std::string> v;
    v.push_back("ardu");
    v.push_back("433MHz");
    v.push_back("");

    std::this_thread::sleep_for(15s);
    command_ardu workerRFLink("ardu", context);

    context->main_RFLink->sendCommand("10;PING;");
    context->main_RFLink->sendCommand("10;PING;");
    context->main_RFLink->sendCommand("10;PING;");

    while (useful_F::go_while)
    {
        std::this_thread::sleep_for(50ms);

        msgFromRFLink = rc433.receiveCode();

        if (msgFromRFLink.size() > 0)
        {
            v[2] = msgFromRFLink;
            workerRFLink.execute(v, context);
        }
    }
    iDOM_THREAD::stop_thread(threadName, context);
}

//////////// watek do obslugi polaczeni miedzy nodami //////////////
void f_serv_con_node(thread_data *context, const std::string &threadName)
{
    context->myEventHandler.run("node")->addEvent("start and stop node");
    // useful_F::clearThreadArray(context);

    iDOM_THREAD::stop_thread(threadName, context);
} // koniec f_serv_con_node

///////////////////// watek MQTT subscriber
void f_master_mqtt(thread_data *context, const std::string &threadName)
{
    bool ex = false;
    try
    {
        context->mqttHandler->connect(context->server_settings->_mqtt_broker.topicSubscribe,
                                      context->server_settings->_mqtt_broker.host);
    }
    catch (const std::string &e)
    {
        ex = true;
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "MQTT: " << e << std::endl;
        log_file_mutex.mutex_unlock();
    }
    if (ex == false)
        context->mqttHandler->subscribeHandlerRunInThread(context->mqttHandler.get());

    iDOM_THREAD::stop_thread(threadName, context);
} // koniec master_mqtt

///////////////////// watek CRON //////////////////////////////
void f_master_CRON(thread_data *context, const std::string &threadName)
{
    while (useful_F::go_while)
    {
        try
        {
            CRON my_CRON(context);
            my_CRON.run();
        }
        catch (const std::exception &e)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << CRITICAL << "THROW in  " << __PRETTY_FUNCTION__ << e.what() << std::endl;
            log_file_mutex.mutex_unlock();
        }
    }

    iDOM_THREAD::stop_thread(threadName, context);
} // koniec CRON

///////////////////// watek influx //////////////////////////////
void f_master_influx(thread_data *context, const std::string &threadName)
{

    while (useful_F::go_while)
    {
        try
        {
            if (context->main_house_room_handler->m_bulbStatus.Empty())
            {
                // sleep
                std::this_thread::sleep_for(1s);
                continue;
            }
            auto data = context->main_house_room_handler->m_bulbStatus.Take();

            dbClientFactory dbFactory;
            auto db = dbFactory.createDbClient();
            auto returnCode = db->uploadBulbData(data.name, data.state);

            if (returnCode != 204)
            {
                log_file_mutex.mutex_lock();
                log_file_cout << CRITICAL << " błąd wysyłania stanu żarówek do influxdb " << returnCode << " " << reasonPhrase(returnCode) << std::endl;
                log_file_mutex.mutex_unlock();
                throw 55;
            }
        }
        catch (const std::exception &e)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << CRITICAL << "THROW in  " << __PRETTY_FUNCTION__ << e.what() << std::endl;
            log_file_mutex.mutex_unlock();
        }
    }

    iDOM_THREAD::stop_thread(threadName, context);
} // koniec influx

///////////////////// watek Satel Integra32 //////////////////////////
void f_satelIntegra32(thread_data *context, const std::string &threadName)
{
    while (useful_F::go_while)
    {
        try
        {
            SATEL_INTEGRA_HANDLER my_integra32(context);
            my_integra32.run();
        }
        catch (const std::exception &e)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << CRITICAL << "THROW in  " << __PRETTY_FUNCTION__ << e.what() << std::endl;
            log_file_mutex.mutex_unlock();
        }
    }

    iDOM_THREAD::stop_thread(threadName, context);
} // koniec Satel Integra32

void my_sig_handler(int s)
{
    printf("\nCaught signal %d\n", s);

    std::cout << "MENU:" << std::endl
              << "0 - STOP" << std::endl
              << "1 - RELOAD" << std::endl;
    int k;
    std::cin >> k;
    std::cout << "podałeś: " << k << std::endl;
    if (useful_F::myStaticData->main_iDomTools != std::nullptr_t())
    {
        if (k == 0)
        {
            std::cout << "zamykam" << std::endl;
            useful_F::myStaticData->main_iDomTools->close_iDomServer();
        }
        else if (k == 1)
        {
            std::cout << "jeszce nie przeladowywuje :(" << std::endl;
            useful_F::myStaticData->main_iDomTools->reloadHard_iDomServer();
        }
        else
            std::cout << "co ty podales?" << std::endl;
    }
}

void catchSigInt()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_sig_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    signal(SIGABRT, cyniu::BACKTRACE::handleCrash);
    signal(SIGSEGV, cyniu::BACKTRACE::handleCrash);
}

iDomStateEnum iDom_main(char *argv[])
{
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "\n\n------------------------ START PROGRAMU -----------------------" << std::endl;
    log_file_mutex.mutex_unlock();

    catchSigInt();
    iDomStateEnum iDomStateProgram = iDomStateEnum::WORKING;
    useful_F::go_while = true;
    std::ofstream pidFile;
    pidFile.open("/mnt/ramdisk/pid-iDom.txt");
    pidFile << getpid();
    pidFile.close();

    pthread_mutex_init(&Logger::mutex_log, NULL);
    // read a JSON file

    CONFIG_JSON server_settings;
    try
    {
        std::ifstream i_config("/etc/config/iDom_SERVER/iDom_serverConfig.json");
        nlohmann::json j;
        i_config >> j;

        server_settings = useful_F::configJsonFileToStruct(j);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n config file error";
    }

    thread_data node_data; // przekazywanie do watku
    node_data.lusina.shedConfJson = server_settings._shedConf;
    node_data.server_settings = &server_settings;
    time(&node_data.start);

    //////////////////////////////////// load json SaveState
    iDom_SAVE_STATE info(node_data.server_settings->_server.saveFilePath);
    nlohmann::json jj;
    try
    {
        jj = info.read();
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "brak pliku z zapisanym stanem iDom" << std::endl;
        log_file_mutex.mutex_unlock();
    }

    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> thread_array;
    for (int i = 0; i < static_cast<int>(thread_array.size()); ++i)
    {
        thread_array[i].thread_name = "  -empty-  ";
        thread_array[i].thread_socket = 0;
    }

    node_data.main_THREAD_arr = &thread_array;

    /////////////////////////////////////////// zaczynam wpisy do logu ////////////////////////////////////////////////////////////
    log_file_mutex.mutex_lock();
    log_file_cout << "\n************************************************\n*****************************************************************\n  " << std::endl;
    log_file_cout << std::boolalpha;
    log_file_cout << INFO << "ID serwera\t" << server_settings._server.ID_server << std::endl;
    log_file_cout << INFO << "PortRS232\t" << server_settings._rs232.portRS232 << std::endl;
    log_file_cout << INFO << "BaudRate RS232\t" << server_settings._rs232.BaudRate << std::endl;
    log_file_cout << INFO << "RFLinkPort\t" << server_settings._rflink.RFLinkPort << std::endl;
    log_file_cout << INFO << "RFLinkBaudRate\t" << server_settings._rflink.RFLinkBaudRate << std::endl;
    log_file_cout << INFO << "port TCP \t" << server_settings._server.PORT << std::endl;
    log_file_cout << INFO << "serwer ip \t" << server_settings._server.SERVER_IP << std::endl;
    log_file_cout << INFO << "klucz ThingSpeak \t" << server_settings._server.TS_KEY << std::endl;
    log_file_cout << INFO << "broker MQTT \t" << server_settings._mqtt_broker.host << std::endl;
    log_file_cout << INFO << "thread MPD \t" << server_settings._runThread.MPD << std::endl;
    log_file_cout << INFO << "thread CRON \t" << server_settings._runThread.CRON << std::endl;
    log_file_cout << INFO << "thread RS232 \t" << server_settings._runThread.RS232 << std::endl;
    log_file_cout << INFO << "thread DUMMY \t" << server_settings._runThread.DUMMY << std::endl;
    log_file_cout << INFO << "thread MQTT \t" << server_settings._runThread.MQTT << std::endl;
    log_file_cout << INFO << "thread RFLink \t" << server_settings._runThread.RFLink << std::endl;
    log_file_cout << INFO << " \n"
                  << std::endl;
    log_file_cout << DEBUG << "zbudowany dnia: " << __DATE__ << " o godzinie: " << __TIME__ << std::endl;

    std::stringstream r;
    const char *PROG_INFO = "iDomServer: " __DATE__ ", " __TIME__;
    r << "wersja " << PROG_INFO << " " << GIT_BRANCH << " " << GIT_COMMIT_HASH << std::endl;

    log_file_cout << DEBUG << r.str() << std::endl;
    log_file_cout << INFO << " \n"
                  << std::endl;
    log_file_mutex.mutex_unlock();

    /////////////////////////////////////////////// koniec logowania do poliku ///////////////////////////////////////////////////

    /////////////////////////////////////////////// start wiringPi //////////////////////////////////////////////
    if (wiringPiSetup() == -1)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "problem z wiringPiSetup()" << std::endl;
        log_file_mutex.mutex_unlock();
        exit(1);
    }

    ///////////////////////////////// tworzenie pliku mkfifo dla sterowania omx playerem
    /*
    int temp = mkfifo("/mnt/ramdisk/cmd",0666);

    if(temp == 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "mkfifo - plik stworzony "<<strerror(errno) << std::endl;
        log_file_mutex.mutex_unlock();
    }
    else if(temp == -1)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "mkfifo - "<<strerror(errno) << std::endl;
        log_file_mutex.mutex_unlock();
    }
*/
    //////////////////////////////////////////////////////////////////////

    useful_F::setStaticData(&node_data);
    /////////////////////////////////////////////////////////

    node_data.sleeper = 0;

    /////////////////////////////////////// MQTT ////////////////////////////
     printf("Running: %s\n", argv[0]);
    node_data.mqttHandler = std::make_unique<MQTT_mosquitto>(argv[0]);
    node_data.mqttHandler->turnOffDebugeMode();

    ////////////////////////////////thread starting part //////////////////////////////////////////////
    /////////////////////////////// RC 433MHz ////////////////////

    node_data.main_REC = std::make_unique<RADIO_EQ_CONTAINER>(&node_data);
    node_data.main_REC->loadConfig(server_settings._server.radio433MHzConfigFile);
    node_data.main_RFLink = std::make_shared<RFLinkHandler>(&node_data);

    if (server_settings._runThread.RFLink == true and node_data.main_RFLink->init())
    {
        // start watku czytania RFLinka
        iDOM_THREAD::start_thread("RFLink thread",
                                  RFLinkHandlerRUN,
                                  &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "nie wystartowalem wątku RFLink" << std::endl;
        log_file_mutex.mutex_unlock();
    }

    ////////////////////////////////////////start watku do komunikacji rs232

    if (server_settings._runThread.RS232 == true)
    {
        iDOM_THREAD::start_thread("RS232 thread",
                                  Send_Recieve_rs232_thread,
                                  &node_data,
                                  1);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "nie wystartowalem wątku RS232" << std::endl;
        log_file_mutex.mutex_unlock();
    }

    /////////////////////////////// MPD info /////////////////////////
    node_data.ptr_MPD_info = std::make_unique<MPD_info>();
    /////////////////////////////// iDom Status //////////////////////
    node_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
    /////////////////////////////// MENU LCD //////////////////////////////
    LCD_c lcd(0x27, 16, 2);
    MENU_STATE_MACHINE stateMechine;
    auto ptr = std::make_unique<MENU_ROOT>(&node_data, &lcd, &stateMechine);
    stateMechine.setStateMachine(std::move(ptr));
    node_data.main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);
    /////////////////////////////// iDom Tools ///////////////////////
    node_data.main_iDomTools = std::make_shared<iDomTOOLS>(&node_data);
    ///////////////////////////////// BUDERUS //////////////////////////////
    node_data.ptr_buderus = std::make_shared<BUDERUS>();
    //////////////////////////////// LIGHT  ///////////////////////////////////
    node_data.main_house_room_handler = std::make_shared<house_room_handler>(&node_data);
    // std::string cf();
    node_data.main_house_room_handler->loadConfig("/etc/config/iDom_SERVER/bulb_config.json");
    node_data.main_house_room_handler->loadButtonConfig("/etc/config/iDom_SERVER/button_config.json");
    //////////////////////////////// SETTINGS //////////////////////////////
    node_data.main_iDomStatus->addObject("house", node_data.idom_all_state.houseState);

    //////////////////////////////////////// start watku MQTT
    if (server_settings._runThread.MQTT == true)
    {
        iDOM_THREAD::start_thread("MQTT thread", f_master_mqtt, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "nie wystartowalem wątku MQTT" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    //////////////////////////////////////// start watku SATEL INTEGRA32
    if (server_settings._runThread.SATEL == true)
    {
        iDOM_THREAD::start_thread("Satel INTEGRA32 thread", f_satelIntegra32, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "nie wystartowalem wątku satel integra32" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    //////////////////////////////////////// start watku mpd_cli
    if (server_settings._runThread.MPD == true)
    {
        iDOM_THREAD::start_thread("MPD  thread", main_mpd_cli, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "nie wystartowalem wątku MPD" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    ///////////////////////////////////////// start watku zarowek do influx
   
    if (server_settings._runThread.INFLUX == true)
    {
        iDOM_THREAD::start_thread("influx thread", f_master_influx, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "nie wystartowalem wątku do uploudu na influxdb" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    ///////////////////////////////////////// start watku CRONa
    if (server_settings._runThread.CRON == true)
    {
        iDOM_THREAD::start_thread("Cron thread", f_master_CRON, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "nie wystartowalem wątku CRON" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    if (server_settings._runThread.DUMMY == true)
    {
        iDOM_THREAD::start_thread("node thread", f_serv_con_node, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "NIE startuje NODA MASTERA do polaczen z innymi " << std::endl;
        log_file_mutex.mutex_unlock();
    }

    ///////////////////////////////////////////////////// INFO PART ////////////////////////////////////////////////
    node_data.main_iDomTools->sendViberMsg("iDom server wystartował", server_settings._fb_viber.viberReceiver.at(0),
                                           server_settings._fb_viber.viberSender);
    /////////////////////////////////////////////////// RESTORE PART ///////////////////////////////////////////////
    node_data.main_iDomTools->readState_iDom(jj);
    ///////////////////////////////////////////////////// TASKER PART ////////////////////////
    TASKER mainTasker(&node_data);
    ///////////////////////////////////////////////////// STARTED //////////////////////////////////////////////////
    node_data.serverStarted = true;
    //////// get light state
    node_data.mqttHandler->publish(house_room_handler::m_mqttPublishTopic, "777;1;0;0");
    ///////////////////////////////////////////////////// start server ////////////////////////////////////////////////////

    useful_F::startServer(&node_data, &mainTasker);

    //////////////////////////////////////////////////// close part ///////////////////////////////////////////////////////
    if (node_data.iDomProgramState == iDomStateEnum::CLOSE or node_data.iDomProgramState == iDomStateEnum::RASPBERRY_RELOAD)
    {
        node_data.main_iDomTools->MPD_stop();
        node_data.main_iDomTools->turnOffSpeakers();
    }

    node_data.mqttHandler->disconnect();

    std::this_thread::sleep_for(100ms);

    pthread_mutex_destroy(&Logger::mutex_log);
    iDomStateProgram = node_data.iDomProgramState;

    useful_F::go_while = false;
    iDOM_THREAD::waitUntilAllThreadEnd(&node_data);

    return iDomStateProgram;
}
