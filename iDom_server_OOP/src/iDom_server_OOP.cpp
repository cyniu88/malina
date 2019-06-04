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
#include "c_master_irda/master_irda.h"
#include "RADIO_433_eq//radio_433_eq.h"
#include "thread_functions/rs232_thread.h"
#include "433MHz/RFLink/rflinkhandler.h"
#include "command/commandClass/command_ardu.h"
#include "thread_functions/iDom_thread.h"
#include "command/commandhandlergateway.h"

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

///////////////////// watek do obslugi irda //////////////////////////////
void f_master_irda (thread_data *my_data, const std::string& threadName){
    master_irda irda(my_data);
    irda.run();
    iDOM_THREAD::stop_thread(threadName, my_data);
} // koniec master_irda

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
        log_file_cout << CRITICAL <<"MQTT: " << e <<std::endl;
        log_file_mutex.mutex_unlock();
    }
    if (ex == false)
        my_data->mqttHandler->subscribeHandlerRunInThread(my_data->mqttHandler);

    iDOM_THREAD::stop_thread(threadName, my_data);
} // koniec master_mqtt


///////////////////// watek CRON //////////////////////////////
void f_master_CRON (thread_data *my_data, const std::string& threadName){
    CRON my_CRON(my_data);
    my_CRON.run();
    iDOM_THREAD::stop_thread(threadName, my_data);
} // koniec CRON

///////////////////// watek polaczenia TCP /////////////////////////////////////
void Server_connectivity_thread(thread_data *my_data, const std::string &threadName){
    C_connection *client = new C_connection(my_data);
    static unsigned int connectionCounter = 0;
    bool key_ok = false;
    std::string tm = inet_ntoa(my_data->from.sin_addr);
    if("192.168.1.1" != tm && my_data->ptr_MPD_info->isPlay == false) {
        my_data->mainLCD->set_print_song_state(32);
        my_data->mainLCD->printString(true,0,0,"USER CONNECTED!");
        my_data->mainLCD->printString(false,0,1,tm);
    }
    if("192.168.1.1" == tm || "192.168.1.144" == tm)
    {
        if(++connectionCounter > 9)
        {
            connectionCounter = 0;
            my_data->main_iDomTools->sendViberMsg("ktoś kombinuje z polaczeniem do serwera!",
                                                  my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                  my_data->server_settings->_fb_viber.viberSender+"_ALERT!");
        }
        client->setEncrypted(false);
    }
    else
    {
        connectionCounter = 0;
    }

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << threadName <<": polaczenie z adresu " << tm <<std::endl;
    log_file_mutex.mutex_unlock();
    my_data->myEventHandler.run("connections")->addEvent(tm);

    int recvSize = client->c_recv(0);
    if(recvSize == -1)
    {
        key_ok = false;
    }
    //std::cout <<"WYNIK:"<< client->c_read_buf().size()<<"a to wlasny" << RSHash().size()<<"!"<<std::endl;
    std::string KEY_OWN = useful_F::RSHash();
    client->setEncriptionKey(KEY_OWN);
    std::string KEY_rec = client->c_read_buf(recvSize);

    if(KEY_rec == KEY_OWN) // stop runing idom_server
    {
        key_ok = true;
        if(client->c_send("OK") == -1)
        {
            puts("FAKE CONNECTION send OK");
            key_ok = false;
        }
    }
    else
    {
        key_ok = false;
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL <<"AUTHENTICATION FAILED! " << inet_ntoa(my_data->from.sin_addr) <<std::endl;
        log_file_cout << CRITICAL <<"KEY RECIVED: " << KEY_rec << " KEY SERVER: "<< KEY_OWN <<std::endl;
        client->cryptoLog(KEY_rec);// setEncriptionKey(KEY_rec);
        log_file_cout << CRITICAL <<"KEY RECIVED\n\n " << KEY_rec <<"\n\n"<< std::endl;
        log_file_mutex.mutex_unlock();

        std::string msg ="podano zły klucz autentykacji - sprawdz logi ";
        msg.append(inet_ntoa(my_data->from.sin_addr));
        my_data->main_iDomTools->sendViberMsg(msg,
                                              my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                              my_data->server_settings->_fb_viber.viberSender+"_ALERT!");
        KEY_rec.clear();

        if(client->c_send("\nFAIL\n") == -1)
        {
            key_ok = false;
            delete client;
            iDOM_THREAD::stop_thread(threadName, my_data);
            return;
        }

    }
    /// ///////////////////////user level
    {
        int recvSize = client->c_recv(0);
        if(recvSize == -1)
        {
            delete client;
            iDOM_THREAD::stop_thread(threadName, my_data);
            return;
        }

        std::string userLevel = client->c_read_buf(recvSize);
        client->c_send("OK you are "+ userLevel);
        puts("user level to:");
        puts(userLevel.c_str());

        if(userLevel == "ROOT")
        {
            client->m_mainCommandHandler = new commandHandlerRoot(my_data);
        }
        else if (userLevel == "GATEWAY") {
            client->m_mainCommandHandler = new commandHandlerGATEWAY(my_data);
        }
        else
        {
            client->m_mainCommandHandler = new commandHandler(my_data);
        }
    }
    while (useful_F::go_while && key_ok)
    {
        int recvSize = client->c_recv(0);
        if(recvSize == -1)
        {
            puts("klient sie rozlaczyl");
            break;
        }
        // ########################### analia wiadomoscu ####################################//
        try
        {
            client->c_analyse(recvSize);
        }
        catch (std::string& s)
        {
            puts("close server - throw");
            useful_F::workServer = false;
            client->c_send("CLOSE");
            break;
        }

        // ############################### koniec analizy wysylanie wyniku do RS232 lub TCP ########################
        if(client->c_send(0) == -1)
        {
            perror("send() ERROR");
            break;
        }
    }
    client->onStopConnection();
    delete client;
    iDOM_THREAD::stop_thread(threadName, my_data);
}

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
    std::ifstream i("/etc/config/iDom_SERVER/iDom_serverConfig.json");
    nlohmann::json j;
    i >> j;
    CONFIG_JSON server_settings = useful_F::configJsonFileToStruct(j);
    struct sockaddr_in server;
    int v_socket;

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

    unsigned int who[2] = {iDomConst::FREE, iDomConst::FREE};
    /////////////////////////////////////////// zaczynam wpisy do logu ////////////////////////////////////////////////////////////
    log_file_mutex.mutex_lock();
    log_file_cout << "\n*****************************************************************\n*****************************************************************\n  "<<  " \t\t\t\t\t start programu " << std::endl;
    log_file_cout << INFO << "ID serwera\t" << server_settings._server.ID_server << std::endl;
    log_file_cout << INFO << "PortRS232\t" << server_settings._rs232.portRS232 << std::endl;
    log_file_cout << INFO << "PortRS232_clock\t" << server_settings._rs232.portRS232_clock << std::endl;
    log_file_cout << INFO << "BaudRate RS232\t" << server_settings._rs232.BaudRate << std::endl;
    log_file_cout << INFO << "RFLinkPort\t" << server_settings._rflink.RFLinkPort << std::endl;
    log_file_cout << INFO << "RFLinkBaudRate\t" << server_settings._rflink.RFLinkBaudRate << std::endl;
    log_file_cout << INFO << "port TCP \t" << server_settings._server.PORT << std::endl;
    log_file_cout << INFO << "serwer ip \t" << server_settings._server.SERVER_IP <<std::endl;
    log_file_cout << INFO << "baza z filami \t" << server_settings._server.MOVIES_DB_PATH << std::endl;
    log_file_cout << INFO << "klucz ThingSpeak \t" <<server_settings._server.TS_KEY << std::endl;
    log_file_cout << INFO << "broker MQTT \t" <<server_settings._mqtt_broker.host << std::endl;
    log_file_cout << INFO << "thread MPD \t" << server_settings._runThread.MPD << std::endl;
    log_file_cout << INFO << "thread CRON \t" << server_settings._runThread.CRON << std::endl;
    log_file_cout << INFO << "thread IRDA \t" << server_settings._runThread.IRDA << std::endl;
    log_file_cout << INFO << "thread RS232 \t" << server_settings._runThread.RS232 << std::endl;
    log_file_cout << INFO << "thread DUMMY \t" << server_settings._runThread.DUMMY << std::endl;
    log_file_cout << INFO << "thread MQTT \t" << server_settings._runThread.MQTT << std::endl;
    log_file_cout << INFO << " \n" << std::endl;
    log_file_cout << INFO << "------------------------ START PROGRAMU -----------------------"<< std::endl;
    log_file_cout << DEBUG << "zbudowany dnia: " << __DATE__ << " o godzinie: " << __TIME__ << std::endl;
    log_file_cout << INFO << " \n" << std::endl;
    log_file_mutex.mutex_unlock();

    /////////////////////////////////////////////// koniec logowania do poliku ///////////////////////////////////////////////////

    /////////////////////////////// RC 433MHz ////////////////////
    node_data.main_REC = std::make_unique<RADIO_EQ_CONTAINER>(&node_data);
    node_data.main_REC->loadConfig(server_settings._server.radio433MHzConfigFile);
    RFLinkHandler rflinkHandler(&node_data);
    bool rflink_work = rflinkHandler.init();
    node_data.main_RFLink = &rflinkHandler;

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
    MPD_info my_MPD_info;
    /////////////////////////////// iDom Status //////////////////////
    iDomSTATUS my_iDomStatus;
    node_data.main_iDomStatus = &my_iDomStatus;
    /////////////////////////////// LCD //////////////////////////////
    LCD_c mainLCD(0x27,16,2);
    ////////////// przegladanie plikow ////////////////////
    files_tree main_tree(server_settings._server.MOVIES_DB_PATH, &mainLCD);
    /////////////////////////////// iDom Tools ///////////////////////
    iDomTOOLS my_iDomTools(&node_data);
    ///////////////////////////////// MENU /////////////////////////////////
    menu_tree main_MENU(server_settings._server.MENU_PATH, &mainLCD);
    //////////////////////////////// SETTINGS //////////////////////////////
    node_data.main_iDomStatus->addObject("house",node_data.idom_all_state.houseState);
    ///////////////////////////////////////////////// wypelniam struktury przesylane do watkow ////////////////////////
    thread_data_rs232 data_rs232;
    data_rs232.BaudRate = server_settings._rs232.BaudRate;
    data_rs232.portRS232 = server_settings._rs232.portRS232;
    data_rs232.portRS232_clock = server_settings._rs232.portRS232_clock;
    data_rs232.pointer.ptr_who = who;

    /////////////////////////////////////// MQTT ////////////////////////////
    MQTT_mosquitto mainMQTT("iDomSERVER");
    mainMQTT.turnOffDebugeMode();
    node_data.mqttHandler = &mainMQTT;

    ////////////////////////////////////////start watku do komunikacji rs232

    if(server_settings._runThread.RS232 == true)
    {
        iDOM_THREAD::start_thread_RS232("RS232 thread",
                                        Send_Recieve_rs232_thread,
                                        &node_data,
                                        &data_rs232,
                                        1);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG <<"nie wystartowalem wątku RS232" <<std::endl;
        log_file_mutex.mutex_unlock();
    }
    ///////////////////////////////// tworzenie pliku mkfifo dla sterowania omx playerem

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

    //////////////////////////////////////////////////////////////////////

    useful_F::setStaticData(&node_data);
    /////////////////////////////////////////////////////////
    int SERVER_PORT = server_settings._server.PORT;
    server_settings._server.SERVER_IP = useful_F::conv_dns(server_settings._server.SERVER_IP);
    const char *SERVER_IP = server_settings._server.SERVER_IP.c_str();
    node_data.pointer.ptr_who = who;
    node_data.mainLCD = &mainLCD;
    node_data.main_tree = &main_tree;
    node_data.main_MENU = &main_MENU;
    node_data.sleeper = 0;
    node_data.ptr_MPD_info = &my_MPD_info;

    pilot_led mainPilotLED;
    node_data.ptr_pilot_led = &mainPilotLED;
    //dodanie pilota
    node_data.main_iDomTools = &my_iDomTools;

    std::unique_ptr <pilot> pilotPTR(new pilot(&node_data.key_map));
    pilotPTR->setup();

    // start watku irda
    if(server_settings._runThread.IRDA == true)
    {
        iDOM_THREAD::start_thread("IRDA thread", f_master_irda, &node_data);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG <<"nie wystartowalem wątku IRDA" <<std::endl;
        log_file_mutex.mutex_unlock();
    }
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
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "BIND problem: " << strerror(errno)<< std::endl;
        log_file_cout << CRITICAL << "awaryjne ! zamykanie gniazda " << shutdown(v_socket, SHUT_RDWR) << std::endl;
        log_file_mutex.mutex_unlock();
        perror("bind() ERROR");
        exit(-1);
    }

    if(listen(v_socket, iDomConst::MAX_CONNECTION) < 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "Listen problem: " << strerror(errno)<< std::endl;
        log_file_mutex.mutex_unlock();
        perror("listen() ERROR");
        exit(-1);
    }
    struct sockaddr_in from;
    ///////////////////////////////////////////////////// INFO PART ////////////////////////////////////////////////
    node_data.main_iDomTools->sendViberMsg("iDom server wystartował", server_settings._fb_viber.viberReceiver.at(0),
                                           server_settings._fb_viber.viberSender);
    /////////////////////////////////////////////////// RESTORE PART ///////////////////////////////////////////////
    node_data.main_iDomTools->readState_iDom(jj);
    ///////////////////////////////////////////////////// TASKER PART ////////////////////////
    TASKER mainTasker(&node_data);
    ///////////////////////////////////////////////////// WHILE ////////////////////////////////////////////////////

    while (1)
    {
        int v_sock_ind = 0;
        memset(&from,0, sizeof(from));
        if(!useful_F::workServer) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        ///////////////////////////////////// TASKER //////////////////////////////////////////
        /// call Tasker
        mainTasker.runTasker();

        if((v_sock_ind = accept(v_socket,(struct sockaddr *) & from, & len)) < 0)
        {
            continue;
        }

        //////////////////////// jest połacznie wiec wstawiamy je do nowego watku i umieszczamy id watku w tablicy w pierwszym wolnym miejscy ////////////////////

        int freeSlotID = iDOM_THREAD::findFreeThreadSlot(&thread_array);

        if(freeSlotID != -1)
        {
            node_data.s_client_sock = v_sock_ind;
            node_data.from = from;
            iDOM_THREAD::start_thread(inet_ntoa(node_data.from.sin_addr),
                                      Server_connectivity_thread,
                                      &node_data,
                                      v_sock_ind);
        }
        else
        {
            log_file_mutex.mutex_lock();
            log_file_cout << INFO << "za duzo klientow "<< thread_array[freeSlotID].thread_ID << std::endl;
            log_file_mutex.mutex_unlock();

            if( (send(v_sock_ind, "za duzo kientow \nEND.\n",22 , MSG_DONTWAIT)) <= 0)
            {
                perror("send() ERROR");
                break;
            }
            continue;
        }
    } // while
    // zamykam gniazdo

    iDomTOOLS::turnOffSpeakers();
    node_data.mainLCD->set_print_song_state(0);
    node_data.mainLCD->set_lcd_STATE(2);
    node_data.mainLCD->clear();
    node_data.mainLCD->noBacklight();
    node_data.main_iDomTools->MPD_stop();
    node_data.mqttHandler->disconnect();
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "zamykanie gniazda wartosc " << shutdown(v_socket, SHUT_RDWR)<< std::endl;
    log_file_cout << ERROR << "gniazdo ind "<<strerror(errno) << std::endl;
    log_file_cout << INFO << "koniec programu "<< std::endl;
    log_file_mutex.mutex_unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    pthread_mutex_destroy(&Logger::mutex_log);
    iDomStateProgram = node_data.iDomProgramState;

    useful_F::go_while = false;
    iDOM_THREAD::waitUntilAllThreadEnd(&node_data);
    return iDomStateProgram;
}

int main(int argc, char *argv[])
{
    iDomStateEnum iDomStateProgram = iDomStateEnum::WORKING;
    std::cout << "startujemy program iDom" << std::endl;

    if (argc == 1)
    {
        int t = 5;
        do
        {
            try
            {
                iDomStateProgram = iDom_main();
            }
            catch (const std::exception& e)
            {
                std::cout << "złąpano wyjatek programu wiec restart "<< e.what() << std::endl;
                iDomStateProgram = iDomStateEnum::RELOAD;
            }
            if(iDomStateProgram == iDomStateEnum::RELOAD)
            {
                std::cout<<std::endl << "przeładowywuje program" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(++t));
            }
        }
        while (iDomStateProgram == iDomStateEnum::RELOAD);

        if(iDomStateProgram == iDomStateEnum::CLOSE)
        {
            std::cout << "zamykam program" << std::endl;
            return 0;
        }
        else if (iDomStateProgram == iDomStateEnum::ERROR)
        {
            std::cout << "zamykam program z ERROREM" << std::endl;
            return 1;
        }
        else if (iDomStateProgram == iDomStateEnum::HARD_RELOAD)
        {
            return 2;
        }
    }
    else
    {
        int ret = 9;
        while (ret != 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            std::cout << "nie ma parametru wiec odpalam program "<< std::endl;
            ret = system("./iDom_server_OOP");
            std::cout << "system() zwraca ret " << ret <<std::endl;
        }
        std::cout << "ZAMYKAM NA AMEN" << std::endl;
    }
    return 0;
}
