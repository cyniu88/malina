#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <vector>
#include <netdb.h>

#include "functions.h"
#include "../thread_functions/iDom_thread.h"
#include "../command/commandhandlergateway.h"
#include "../command/commandhandlerroot.h"
#include "../c_connection/c_connection.h"
#include "../iDomTools/idomtools_interface.h"

std::vector<std::string> useful_F::split(const std::string &s, char separator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(separator, pos)) not_eq std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        output.push_back(substring);
        prev_pos = ++pos;
    }
    try
    {
        output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek substr() w useful_F::split() !!!!!!" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    return output;
}

thread_data *useful_F::myStaticData = std::nullptr_t();

void useful_F::setStaticData(thread_data *my_dataPtr)
{
    myStaticData = my_dataPtr;
}

void useful_F::tokenizer(std::vector<std::string> &command,
                         const std::string &separator,
                         const std::string &text)
{
    std::string temp;

    for (const char& n : text)
    { // the initializer may be an array


        if (!std::any_of(separator.begin(), separator.end(), [n](int y) { return n == y; }))
            temp.push_back(n);
        else
        {
            if (!temp.empty())
            {
                command.push_back(temp);
                temp.clear();
            }
        }
    }
    if (!temp.empty())
        command.push_back(temp);

    if (command.size() == 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << " throw useful_F::tokenizer ()" << std::endl;
        log_file_mutex.mutex_unlock();
        throw std::string("command empty");
    }
}

////// watek sleeper
void useful_F::sleeper_mpd(thread_data *my_data, const std::string &threadName)
{
    my_data->main_Rs232->print("LED_CLOCK:1;");
    unsigned int t = 60 / my_data->sleeper;
    unsigned int k = 0;

    for (; my_data->sleeper > 0; my_data->sleeper--)
    {
        useful_F::sleep(60s);
        k += t;
        my_data->main_iDomTools->ledClear(0, k);
    }
    my_data->main_iDomTools->ledOFF();
    my_data->main_iDomTools->MPD_stop();
    my_data->main_iDomTools->turnOff433MHzSwitch("listwa");

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "zaczynam procedure konca watku " << threadName << std::endl;
    log_file_mutex.mutex_unlock();

    my_data->main_Rs232->print("LED_CLOCK:0;");

    iDOM_THREAD::stop_thread(threadName, my_data);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "koniec watku SLEEP_MPD" << std::endl;
    log_file_mutex.mutex_unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////// watek kodi
void useful_F::kodi(thread_data *my_data, const std::string &threadName)
{
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "start wątku " << threadName << std::endl;
    log_file_mutex.mutex_unlock();

    // my_data->mainLCD->set_print_song_state(100);
    // my_data->mainLCD->printString(false,2,1,"  KODI");

    my_data->main_iDomStatus->setObjectState("KODI", STATE::ACTIVE);
    // status mpd
    STATE musicState = my_data->main_iDomStatus->getObjectState("music");
    // status glosnikow
    STATE speakersState = my_data->main_iDomStatus->getObjectState("speakers");

    if (musicState not_eq STATE::STOP)
        my_data->main_iDomTools->MPD_pause();
    if (speakersState not_eq STATE::ON)
        my_data->main_iDomTools->turnOnSpeakers();
    // system

    int ret = useful_F::runLinuxCommand("runuser -u pi kodi");

    if (ret not_eq 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "kodi zamkniete z błędem " << ret << std::endl;
        log_file_mutex.mutex_unlock();
    }
    // przywracanie danych

    if (musicState == STATE::PLAY)
        my_data->main_iDomTools->MPD_play(my_data);
    else
        my_data->main_iDomTools->turnOffSpeakers();
    // koniec
    my_data->main_key_menu_handler->timeout();
    my_data->main_iDomStatus->setObjectState("KODI", STATE::DEACTIVE);
    iDOM_THREAD::stop_thread("kodi smartTV", my_data);
}
std::string useful_F::RSHash(const std::string &data, unsigned int b, unsigned int a)
{
    time_t act_time;
    struct tm *act_date;
    time(&act_time);
    act_date = localtime(&act_time);
    char buffer3[10];
    strftime(buffer3, 10, "%M%H%w", act_date);
    std::string str(buffer3);
    str.append(data);
    unsigned int hash = 0;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        hash = hash * a + str[i];
        a = a * b;
    }
    return std::to_string((hash & 0x7FFFFFFF));
}

std::string useful_F::sek_to_uptime(long long secy)
{
    const int min = 60;     // s
    const int houry = 3600; // s
    const int day = 86400;  // s
    int number_day, number_hour, number_min, number_sec;
    int temp1, temp2, temp3;
    number_day = secy / day;
    std::stringstream text;
    text << "\n"
         << number_day << " day ";
    temp1 = secy % day;
    number_hour = temp1 / houry;
    text << number_hour << " hours ";
    temp2 = temp1 % houry;
    number_min = temp2 / min;
    text << number_min << " minutes ";
    temp3 = temp2 % min;
    number_sec = temp3;
    text << number_sec << " seconds";
    return text.str();
}

// wysylanie pliku
std::string useful_F::l_send_file(const std::string &path, const std::string &find, bool reverse)
{
    std::string str_buf;
    if (find.empty() == true)
    {
        std::fstream log_file;
        log_file.open(path.c_str(), std::ios::in);
        if (log_file.good() == false)
        {
            return " can not open file !";
        }
        else
        {
            str_buf.erase();
            while (!log_file.eof())
            {
                str_buf.push_back(static_cast<char>(log_file.get()));
            }
            str_buf.erase(str_buf.length() - 1, str_buf.length());

            log_file.close();
        }
    }
    else
    {
        std::fstream log_file;
        log_file.open(path.c_str(), std::ios::in);
        if (log_file.good() == false)
        {
            return " can not open file !";
        }
        else
        {
            str_buf.erase();
            std::string str_temp;
            while (std::getline(log_file, str_temp))
            {
                if (reverse)
                {
                    if (std::string::npos not_eq str_temp.find(find))
                    {
                        str_buf.append(str_temp);
                        str_buf.append("\n");
                    }
                    else
                    {
                        if (str_buf.size() < 3)
                        {
                            str_buf.append("    ");
                        }
                    }
                }
                else
                {
                    if (std::string::npos == str_temp.find(find))
                    {
                        str_buf.append(str_temp);
                        str_buf.append("\n");
                    }
                }
                if (str_buf.size() < 3)
                {
                    str_buf.append("    ");
                }
            }
        }
        log_file.close();
    }
    return str_buf;
}

///////////////////// watek polaczenia TCP /////////////////////////////////////
void useful_F::Server_connectivity_thread(thread_data *my_data, const std::string &threadName)
{
    auto client = std::make_unique<C_connection>(my_data);
    static unsigned int connectionCounter = 0;
    bool key_ok = false;
    std::string tm = inet_ntoa(my_data->from.sin_addr);

    if (tm.find("192.168.13.18") != std::string::npos)
    {
        if (++connectionCounter > 9)
        {
            connectionCounter = 0;
            my_data->main_iDomTools->sendViberMsg("ktoś kombinuje z polaczeniem do serwera!",
                                                  my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                  my_data->server_settings->_fb_viber.viberSender + "_ALERT!");
        }
        client->setEncrypted(false);
    }
    else
    {
        connectionCounter = 0;
    }

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << threadName << ": polaczenie z adresu " << tm << std::endl;
    log_file_mutex.mutex_unlock();
    my_data->myEventHandler.run("connections")->addEvent(tm);
    my_data->main_Rs232->print("LED_AT:1;");

    int recvSize_tm = client->c_recv(0);

    if (recvSize_tm == -1)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "CLOSE, AUTHENTICATION FAILED! " << inet_ntoa(my_data->from.sin_addr) << std::endl;
        log_file_mutex.mutex_unlock();

        my_data->main_Rs232->print("LED_AT:0;");
        iDOM_THREAD::stop_thread(threadName, my_data);
        return;
    }
    std::string KEY_OWN = useful_F::RSHash();
    client->setEncriptionKey(KEY_OWN);
    std::string KEY_rec = client->c_read_buf(recvSize_tm);

    if (KEY_rec == KEY_OWN) // stop runing idom_server
    {
        key_ok = true;
        if (client->c_send("OK") == -1)
        {
            std::cout << "FAKE CONNECTION send OK" << std::endl;
            key_ok = false;
        }
    }
    else
    {
        auto KEY_rec2 = KEY_rec;
        client->cryptoLog(KEY_rec2);

        if (useful_F_libs::hasSubstring(KEY_rec2, "HTTP"))
        {
            client->hendleHTTP(KEY_rec2);

            my_data->main_Rs232->print("LED_AT:0;");
            iDOM_THREAD::stop_thread(threadName, my_data);
            return;
        }

        key_ok = false;
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "AUTHENTICATION FAILED! " << inet_ntoa(my_data->from.sin_addr) << std::endl;
        log_file_cout << CRITICAL << "KEY RECIVED: " << KEY_rec << " KEY SERVER: " << KEY_OWN << std::endl;
        client->cryptoLog(KEY_rec); // setEncriptionKey(KEY_rec);
        log_file_cout << CRITICAL << "KEY UNCRIPTED RECIVED\n\n " << KEY_rec << "\n\n"
                      << std::endl;
        log_file_mutex.mutex_unlock();

        std::string msg = "podano zły klucz autentykacji - sprawdz logi ";
        msg.append(inet_ntoa(my_data->from.sin_addr));
        my_data->main_iDomTools->sendViberMsg(msg,
                                              my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                              my_data->server_settings->_fb_viber.viberSender + "_ALERT!");

        if (client->c_send("\nFAIL\n") == -1)
        {
            my_data->main_Rs232->print("LED_AT:0;");
            iDOM_THREAD::stop_thread(threadName, my_data);
            return;
        }
    }
    /// ///////////////////////user level
    {
        int recvSize_tm_n = client->c_recv(0);
        if (recvSize_tm_n == -1)
        {
            my_data->main_Rs232->print("LED_AT:0;");
            iDOM_THREAD::stop_thread(threadName, my_data);
            return;
        }

        std::string userLevel = client->c_read_buf(recvSize_tm_n);
        client->c_send("OK you are " + userLevel);

        if (userLevel == "ROOT")
        {
            client->m_mainCommandHandler = std::make_unique<commandHandlerRoot>(my_data);
        }
        else if (userLevel == "GATEWAY")
        {
            client->m_mainCommandHandler = std::make_unique<commandHandlerGATEWAY>(my_data);
        }
        else
        {
            client->m_mainCommandHandler = std::make_unique<commandHandler>(my_data);
        }
    }
    while (useful_F::go_while && key_ok)
    {
        int recvSize_tm_n = client->c_recv(0);
        if (recvSize_tm_n == -1)
        {
            puts("klient sie rozlaczyl");
            break;
        }
        // ########################### analia wiadomoscu ####################################//
        try
        {
            client->c_analyse(recvSize_tm_n);
        }
        catch (std::string &s)
        {
            puts("close server - throw");
            useful_F::workServer = false;
            client->c_send("CLOSE");
            break;
        }
        catch (const std::exception &e)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << CRITICAL << "złapano wyjatek: " << e.what() << std::endl;
            log_file_mutex.mutex_unlock();
            break;
        }

        // ############################### koniec analizy wysylanie wyniku do RS232 lub TCP ########################
        if (client->c_send(0) == -1)
        {
            perror("send() ERROR");
            break;
        }
    }
    client->onStopConnection();
    my_data->main_Rs232->print("LED_AT:0;");
#ifdef BT_TEST
    std::cout << "zamykamy server" << std::endl;
    useful_F::workServer = false; // wylacz nasluchwianie servera
#endif
    iDOM_THREAD::stop_thread(threadName, my_data);
}

// przerobka adresu na ip . //////////////////////////////////
std::string useful_F::conv_dns(std::string &temp)
{

    int i;
    struct hostent *he;
    struct in_addr **addr_list;
    std::string s_ip;

    if ((he = gethostbyname(temp.c_str())) == NULL)
    {
        herror("gethostbyname");
        return "- 1";
    }
    // print information about this host:
    printf("Official name is: %s\n", he->h_name);
    printf("IP addresses: ");
    addr_list = (struct in_addr **)(he->h_addr_list);

    for (i = 0; addr_list[i] not_eq NULL; i++)
    {
        printf("%s ", inet_ntoa(*addr_list[i]));
        s_ip += inet_ntoa(*addr_list[i]);
    }
    printf("\ndone ");
    return s_ip;
}

void useful_F::startServer(thread_data *my_data, TASKER *my_tasker)
{
    struct sockaddr_in server;
    int v_socket;
    int SERVER_PORT = my_data->server_settings->_server.PORT;
    my_data->server_settings->_server.SERVER_IP =
        useful_F::conv_dns(my_data->server_settings->_server.SERVER_IP);
    const char *SERVER_IP = my_data->server_settings->_server.SERVER_IP.c_str();

    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0)
    {
        perror("inet_pton() ERROR");
        exit(-1);
    }

    if ((v_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket() ERROR");
        exit(-1);
    }

    if (fcntl(v_socket, F_SETFL, O_NONBLOCK) < 0) // fcntl()
    {
        perror("fcntl() ERROR");
        exit(-1);
    }
    // zgub wkurzający komunikat błędu "address already in use"
    int yes = 1;
    if (setsockopt(v_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }
    socklen_t len = sizeof(server);
    if (bind(v_socket, (struct sockaddr *)&server, len) < 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "BIND problem: " << strerror(errno) << std::endl;
        log_file_cout << CRITICAL << "awaryjne ! zamykanie gniazda " << shutdown(v_socket, SHUT_RDWR) << std::endl;
        log_file_mutex.mutex_unlock();
        perror("bind() ERROR");
        exit(-1);
    }

    if (listen(v_socket, iDomConst::MAX_CONNECTION) < 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "Listen problem: " << strerror(errno) << std::endl;
        log_file_mutex.mutex_unlock();
        perror("listen() ERROR");
        exit(-1);
    }
    struct sockaddr_in from;
    my_data->main_Rs232->print("LED_POWER:1;");
    while (1)
    {
        int v_sock_ind = 0;
        memset(&from, 0, sizeof(from));
        if (!useful_F::workServer)
        {
            break;
        }

        ///////////////////////////////////// TASKER //////////////////////////////////////////
        /// call Tasker, if Tasker has a lot of work to do, we cut waiting times
        int delayMS = my_tasker->runTasker();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMS));

        if ((v_sock_ind = accept(v_socket, (struct sockaddr *)&from, &len)) < 0)
        {
            continue;
        }

        //////////////////////// jest połacznie wiec wstawiamy je do nowego watku i umieszczamy id watku w tablicy w pierwszym wolnym miejscu ////////////////////

        int freeSlotID = iDOM_THREAD::findFreeThreadSlot(my_data->main_THREAD_arr);

        if (freeSlotID not_eq -1)
        {
            my_data->s_client_sock = v_sock_ind;
            my_data->from = from;
            iDOM_THREAD::start_thread(inet_ntoa(my_data->from.sin_addr),
                                      useful_F::Server_connectivity_thread,
                                      my_data,
                                      v_sock_ind);
        }
        else
        {
            log_file_mutex.mutex_lock();
            log_file_cout << INFO << "za duzo klientow " << std::endl;
            log_file_mutex.mutex_unlock();

            if ((send(v_sock_ind, "za duzo kientow \nEND.\n", 22, MSG_DONTWAIT)) <= 0)
            {
                perror("send() ERROR");
                break;
            }
            continue;
        }
    } // while
    close(v_socket);

    my_data->main_Rs232->print("LED_POWER:0;");
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "zamykanie gniazda wartosc " << shutdown(v_socket, SHUT_RDWR) << std::endl;
    log_file_cout << ERROR << "gniazdo ind " << strerror(errno) << std::endl;
    log_file_cout << INFO << "koniec programu " << std::endl;
    log_file_mutex.mutex_unlock();
    // zamykam gniazdo
}

CONFIG_JSON useful_F::configJsonFileToStruct(nlohmann::json jj)
{
    CONFIG_JSON cj;
#ifdef BT_TEST
    std::cout << "json: " << jj.dump(4);
#endif /////////////////////// server settings
    cj._server.PORT = jj["Server_settings"].at("PORT").get<int>();
    cj._server.encrypted = jj["Server_settings"].at("ENCRYPTED").get<bool>();
    cj._server.ID_server = jj["Server_settings"].at("ID").get<int>();
    cj._server.SERVER_IP = jj["Server_settings"].at("SERVER_IP").get<std::string>();
    cj._server.saveFilePath = jj["Server_settings"].at("saveFilePath").get<std::string>();
    cj._server.radio433MHzConfigFile = jj["433MHz_settings"].at("433MHz_config").get<std::string>();
    cj._server.ftpServer.URL = jj["FTP_settings"].at("FTP_URL").get<std::string>();
    cj._server.ftpServer.user = jj["FTP_settings"].at("FTP_LOGIN").get<std::string>();
    cj._server.ftpServer.pass = jj["FTP_settings"].at("FTP_PASS").get<std::string>();
    cj._server.TS_KEY = jj["THINGSPEAK_settings"].at("TS_KEY").get<std::string>();
    cj._server.TS_KEY2 = jj["THINGSPEAK_settings"].at("TS_KEY2").get<std::string>();
    cj._server.lightningApiURL = jj["lightning_settings"].at("LIGHTNING_API_URL").get<std::string>();
    cj._server.MPD_IP = jj["MPD_settings"].at("MPD_IP").get<std::string>();
    cj._server.keyDatabasePath = jj["Server_settings"].at("key_DB_PATH").get<std::string>();
    /////////////////////// viber
    cj._fb_viber.viberToken = jj["viber_settings"].at("VIBER_TOKEN").get<std::string>();
    cj._fb_viber.viberAvatar = jj["viber_settings"].at("VIBER_AVATAR").get<std::string>();
    cj._fb_viber.viberSender = jj["viber_settings"].at("VIBER_SENDER").get<std::string>();
    for (auto &k : jj["viber_settings"].at("VIBER_RECEIVER"))
    {
        cj._fb_viber.viberReceiver.push_back(k.get<std::string>());
    }
    ////////////////////// facebook
    cj._fb_viber.facebookAccessToken = jj["facebook_settings"].at("FB_ACCESS_TOKEN").get<std::string>();
    /////////////////////// thread settings
    cj._runThread.MPD = jj["THREAD"].at("MPD").at("run").get<bool>();
    cj._runThread.RFLink = jj["THREAD"].at("RFLink").at("run").get<bool>();
    cj._runThread.CRON = jj["THREAD"].at("CRON").at("run").get<bool>();
    cj._runThread.RS232 = jj["THREAD"].at("RS232").at("run").get<bool>();
    cj._runThread.DUMMY = jj["THREAD"].at("DUMMY").at("run").get<bool>();
    cj._runThread.MQTT = jj["THREAD"].at("MQTT").at("run").get<bool>();
    cj._runThread.SATEL = jj["THREAD"].at("SATEL").at("run").get<bool>();
    /////////////////////// camera
    cj._camera.cameraLedOFF = jj["camera_settings"].at("CAMERA_LED_OFF").get<std::string>();
    cj._camera.cameraLedON = jj["camera_settings"].at("CAMERA_LED_ON").get<std::string>();
    cj._camera.cameraURL = jj["camera_settings"].at("CAMERA_SNAPSHOT").get<std::string>();
    /////////////////////// mqtt broker
    cj._mqtt_broker.qos = jj["mqtt_broker_settings"].at("qos").get<int>();
    cj._mqtt_broker.port = jj["mqtt_broker_settings"].at("port").get<int>();
    cj._mqtt_broker.host = jj["mqtt_broker_settings"].at("host").get<std::string>();
    cj._mqtt_broker.topicPublish = jj["mqtt_broker_settings"].at("publish topic").get<std::string>();
    cj._mqtt_broker.topicSubscribe = jj["mqtt_broker_settings"].at("subscribe topic").get<std::string>();
    /////////////////////// rs232
    cj._rs232.BaudRate = jj["RS232_settings"].at("BaudRate").get<int>();
    cj._rs232.portRS232 = jj["RS232_settings"].at("portRS232").get<std::string>();
    /////////////////////// RFLink
    cj._rflink.RFLinkPort = jj["RFLink_settings"].at("RFLinkPort").get<std::string>();
    cj._rflink.RFLinkBaudRate = jj["RFLink_settings"].at("RFLinkBaudRate").get<int>();
    /////////////////////// gateway
    cj._gateway.url = jj["gateway"].at("url").get<std::string>();
    cj._gateway.keySize = jj["gateway"].at("key_size").get<unsigned int>();
    /////////////////////// SATEL INTEGRA32
    cj._satel_integra.host = jj["satel_integra32"].at("host").get<std::string>();
    cj._satel_integra.port = jj["satel_integra32"].at("port").get<int>();
    cj._satel_integra.pin = jj["satel_integra32"].at("pin").get<std::string>();
    cj._satel_integra.outdoor_siren_lights_id = jj["satel_integra32"].at("outdoor_siren_lights_id").get<unsigned int>();
    ///////////////////////// SHED //////////////////////////////////////////////////////////////////////////////
    cj._shedConf = jj["shed"];
    ///////////////////////// COMMAND ////////////////////////////////////////////////////
    cj._command = jj["command"];

    return cj;
}

std::map<std::string, iDom_API *> iDom_API::m_map_iDom_API;
std::mutex iDom_API::m_locker;

std::string iDom_API::getDump()
{
    std::lock_guard<std::mutex> lock(iDom_API::m_locker);
    std::stringstream ret;
    for (const auto &it : m_map_iDom_API)
    {
        ret << std::endl
            << "----------------------------"
            << std::endl
            << " map element: " << it.first << std::endl
            << it.second->dump();
    }
    return ret.str();
}

void iDom_API::addToMap(const std::string &name, iDom_API *ptr)
{
    std::lock_guard<std::mutex> lock(iDom_API::m_locker);
    m_map_iDom_API.insert(std::make_pair(name, ptr));
}

void iDom_API::removeFromMap(const std::string &name)
{
    std::lock_guard<std::mutex> lock(iDom_API::m_locker);
    if (m_map_iDom_API.erase(name) == 0)
    {
        puts("nie ma w MAPIE !!");
        puts(name.c_str());
    }
}
