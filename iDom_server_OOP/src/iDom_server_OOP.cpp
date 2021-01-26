#include "iDom_server_OOP.h"
#include "functions/functions.h"
#include <wiringPi.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "CRON/cron.hpp"
#include "functions/functions.h"
#include "functions/mpd_cli.h"
#include "c_connection/c_connection.h"
#include "TASKER/tasker.h"
#include "RADIO_433_eq//radio_433_eq.h"
#include "thread_functions/rs232_thread.h"
#include "433MHz/RFLink/rflinkhandler.h"
#include "command/commandClass/command_ardu.h"
#include "thread_functions/iDom_thread.h"
#include "command/commandhandlergateway.h"
#include "MENU/menu_root.h"

std::string _logfile = "/mnt/ramdisk/iDom_log.log";
std::string buffer;
Logger log_file_mutex(_logfile);

//////////////// watek RFLink //////////////////////////////////
void RFLinkHandlerRUN(thread_data *my_data, const std::string& threadName){
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "watek "<< threadName<< " wystartowal "<< std::this_thread::get_id() << std::endl;
    log_file_mutex.mutex_unlock();
    std::string msgFromRFLink;
    RC_433MHz rc433(my_data);
    my_data->main_RFLink->flush();
    std::vector <std::string>  v;
    v.push_back("ardu");
    v.push_back("433MHz");
    v.push_back("");

    std::this_thread::sleep_for(std::chrono::seconds(5));
    command_ardu workerRFLink("ardu",my_data);

    my_data->main_RFLink->sendCommand("10;PING;");

    while(useful_F::go_while){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        msgFromRFLink = rc433.receiveCode();

        if (msgFromRFLink.size() > 0){
            v[2] = msgFromRFLink;
            workerRFLink.execute(v, my_data);
        }

    }
    iDOM_THREAD::stop_thread(threadName, my_data);
}

//////////// watek do obslugi polaczeni miedzy nodami //////////////
void f_serv_con_node (thread_data *my_data, const std::string& threadName){
    my_data->myEventHandler.run("node")->addEvent("start and stop node");
    //useful_F::clearThreadArray(my_data);

    iDOM_THREAD::stop_thread(threadName,my_data);
} // koniec f_serv_con_node


///////////////////// watek MQTT subscriber
void f_master_mqtt (thread_data *my_data, const std::string& threadName){
    bool ex = false;
    try{
        my_data->mqttHandler->connect(my_data->server_settings->_mqtt_broker.topicSubscribe,
                                      my_data->server_settings->_mqtt_broker.host);}
    catch (const std::string& e)
    {
        ex = true;
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "MQTT: " << e <<std::endl;
        log_file_mutex.mutex_unlock();
    }
    if (ex == false)
        my_data->mqttHandler->subscribeHandlerRunInThread(my_data->mqttHandler.get());

    iDOM_THREAD::stop_thread(threadName, my_data);
} // koniec master_mqtt


///////////////////// watek CRON //////////////////////////////
void f_master_CRON (thread_data *my_data, const std::string& threadName){
    CRON my_CRON(my_data);
    my_CRON.run();
    iDOM_THREAD::stop_thread(threadName, my_data);
} // koniec CRON


iDomStateEnum iDom_main()
{
    iDomStateEnum iDomStateProgram = iDomStateEnum::CLOSE;
    useful_F::go_while = true;
    std::ofstream pidFile;
    pidFile.open("/mnt/ramdisk/pid-iDom.txt");
    pidFile << getpid();
    pidFile.close();

    pthread_mutex_init(&Logger::mutex_log, NULL);
    // read a JSON file
    std::ifstream i_config("/etc/config/iDom_SERVER/iDom_serverConfig.json");
    nlohmann::json j;
    i_config >> j;
    CONFIG_JSON server_settings = useful_F::configJsonFileToStruct(j);


    thread_data node_data; // przekazywanie do watku
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
        log_file_cout << DEBUG <<"brak pliku z zapisanym stanem iDom" << std::endl;
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
    log_file_cout << "\n*****************************************************************\n*****************************************************************\n  "<<  " \t\t\t\t\t start programu " << std::endl;
    log_file_cout << INFO << "ID serwera\t" << server_settings._server.ID_server << std::endl;
    log_file_cout << INFO << "PortRS232\t" << server_settings._rs232.portRS232 << std::endl;
    log_file_cout << INFO << "BaudRate RS232\t" << server_settings._rs232.BaudRate << std::endl;
    log_file_cout << INFO << "RFLinkPort\t" << server_settings._rflink.RFLinkPort << std::endl;
    log_file_cout << INFO << "RFLinkBaudRate\t" << server_settings._rflink.RFLinkBaudRate << std::endl;
    log_file_cout << INFO << "port TCP \t" << server_settings._server.PORT << std::endl;
    log_file_cout << INFO << "serwer ip \t" << server_settings._server.SERVER_IP <<std::endl;
    log_file_cout << INFO << "klucz ThingSpeak \t" <<server_settings._server.TS_KEY << std::endl;
    log_file_cout << INFO << "broker MQTT \t" <<server_settings._mqtt_broker.host << std::endl;
    log_file_cout << INFO << "thread MPD \t" << server_settings._runThread.MPD << std::endl;
    log_file_cout << INFO << "thread CRON \t" << server_settings._runThread.CRON << std::endl;
    log_file_cout << INFO << "thread RS232 \t" << server_settings._runThread.RS232 << std::endl;
    log_file_cout << INFO << "thread DUMMY \t" << server_settings._runThread.DUMMY << std::endl;
    log_file_cout << INFO << "thread MQTT \t" << server_settings._runThread.MQTT << std::endl;
    log_file_cout << INFO << " \n" << std::endl;
    log_file_cout << INFO << "------------------------ START PROGRAMU -----------------------"<< std::endl;
    log_file_cout << DEBUG << "zbudowany dnia: " << __DATE__ << " o godzinie: " << __TIME__ << std::endl;

    std::stringstream r;
    const char * PROG_INFO = "iDomServer: " __DATE__ ", " __TIME__;
    r << "wersja " <<PROG_INFO <<" "<< GIT_BRANCH <<" " << GIT_COMMIT_HASH << std::endl;

    log_file_cout << DEBUG <<  r.str()  << std::endl;
    log_file_cout << INFO << " \n" << std::endl;
    log_file_mutex.mutex_unlock();

    /////////////////////////////////////////////// koniec logowania do poliku ///////////////////////////////////////////////////

    /////////////////////////////// RC 433MHz ////////////////////
    node_data.main_REC = std::make_unique<RADIO_EQ_CONTAINER>(&node_data);
    node_data.main_REC->loadConfig(server_settings._server.radio433MHzConfigFile);
    //std::unique_ptr<RFLinkHandler> rflinkHandler(new RFLinkHandler(&node_data));
    node_data.main_RFLink = std::make_shared<RFLinkHandler>(&node_data);
    bool rflink_work = node_data.main_RFLink->init();
    // node_data.main_RFLink = std::move(rflinkHandler);

    if (rflink_work == true){
        //start watku czytania RFLinka
        iDOM_THREAD::start_thread("RFLink thread",
                                  RFLinkHandlerRUN,
                                  &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG <<"nie wystartowalem wątku RFLink" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    /////////////////////////////////////////////// start wiringPi //////////////////////////////////////////////
    if(wiringPiSetup() == -1)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL <<"problem z wiringPiSetup()" << std::endl;
        log_file_mutex.mutex_unlock();
        exit(1);
    }

    /////////////////////////////// MPD info /////////////////////////
    node_data.ptr_MPD_info = std::make_unique<MPD_info>();
    /////////////////////////////// iDom Status //////////////////////
    node_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
    /////////////////////////////// MENU LCD //////////////////////////////
    LCD_c lcd(0x27,16,2);
    MENU_STATE_MACHINE stateMechine;
    auto ptr = std::make_unique<MENU_ROOT>(&node_data, &lcd, &stateMechine);
    stateMechine.setStateMachine(std::move(ptr));
    node_data.main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);
    /////////////////////////////// iDom Tools ///////////////////////
    node_data.main_iDomTools = std::make_unique <iDomTOOLS>(&node_data);
    ///////////////////////////////// BUDERUS //////////////////////////////
    node_data.ptr_buderus = std::make_unique<BUDERUS>();
    //////////////////////////////// LIGHY  ///////////////////////////////////
    node_data.main_house_lighting_handler = std::make_shared<house_lighting_handler>(&node_data);
    std::string cf("/etc/config/iDom_SERVER/bulb_config.json");
    node_data.main_house_lighting_handler->loadConfig(cf);
    //////////////////////////////// SETTINGS //////////////////////////////
    node_data.main_iDomStatus->addObject("house",node_data.idom_all_state.houseState);

    /////////////////////////////////////// MQTT ////////////////////////////
    node_data.mqttHandler = std::make_unique<MQTT_mosquitto>("iDomSERVER");
    node_data.mqttHandler->turnOffDebugeMode();

    ////////////////////////////////////////start watku do komunikacji rs232

    if(server_settings._runThread.RS232 == true)
    {
        iDOM_THREAD::start_thread("RS232 thread",
                                  Send_Recieve_rs232_thread,
                                  &node_data,
                                  1);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG <<"nie wystartowalem wątku RS232" <<std::endl;
        log_file_mutex.mutex_unlock();
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

    // start watku MQTT
    if(server_settings._runThread.MQTT == true)
    {
        iDOM_THREAD::start_thread("MQTT thread", f_master_mqtt, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG <<"nie wystartowalem wątku MQTT" <<std::endl;
        log_file_mutex.mutex_unlock();
    }
    // start watku mpd_cli
    if(server_settings._runThread.MPD == true)
    {
        iDOM_THREAD::start_thread("MPD  thread",main_mpd_cli, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG <<"nie wystartowalem wątku MPD" <<std::endl;
        log_file_mutex.mutex_unlock();
    }
    // start watku CRONa
    if(server_settings._runThread.CRON == true)
    {
        iDOM_THREAD::start_thread("Cron thread",f_master_CRON, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG <<"nie wystartowalem wątku CRON" <<std::endl;
        log_file_mutex.mutex_unlock();
    }
    if(server_settings._runThread.DUMMY == true){
        iDOM_THREAD::start_thread("node thread",f_serv_con_node,&node_data);
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
    ///////////////////////////////////////////////////// start server ////////////////////////////////////////////////////
    node_data.main_Rs232->print("LED_POWER:1;");

    useful_F ::startServer(&node_data, &mainTasker);

    if (node_data.iDomProgramState == iDomStateEnum::CLOSE)
    {
        node_data.main_iDomTools->MPD_stop();
        iDomTOOLS::turnOffSpeakers();
    }
    node_data.mqttHandler->disconnect();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    pthread_mutex_destroy(&Logger::mutex_log);
    iDomStateProgram = node_data.iDomProgramState;

    useful_F::go_while = false;
    iDOM_THREAD::waitUntilAllThreadEnd(&node_data);
    return iDomStateProgram;
}
