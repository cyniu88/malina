#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <chrono>
#include <array>
#include <atomic>

// MOJE BIBLIOTEKI
#include "logger/logger.hpp"
#include "../libs/event_counters/event_counters_handler.h"
#include "../libs/alarm/alarm.h"
#include "../libs/Statistic/statistic.h"
#include "iDomStatus/idomstatus.h"
#include "../mqtt_mosquitto/MQTT_mosquitto/mqtt.h"
#include "buderus/buderus_interface.h"
#include "HOUSE/house_room_handler.h"
#include "SerialPi/serialpi.h"
#include "SATEL_INTEGRA/satel_integra_interface.h"
#include "iDomKEY_ACCESS/idomkey_access.h"

#define log_file_cout f_log // std::cout zmien f_log na std::cout i bedzie wypisywac na ekran
#define log_file_mutex f_log


enum class iDomStateEnum
{
    CLOSE = 0,
    RELOAD,
    ERROR,
    WORKING,
    HARD_RELOAD,
    RASPBERRY_RELOAD
};
inline std::ostream &operator<<(std::ostream &os, const iDomStateEnum &v)
{
    switch (v)
    {
    case iDomStateEnum::CLOSE:
        os << "CLOSE";
        break;
    case iDomStateEnum::RELOAD:
        os << "RELOAD";
        break;
    case iDomStateEnum::ERROR:
        os << "ERROR";
        break;
    case iDomStateEnum::WORKING:
        os << "WORKING";
        break;
    case iDomStateEnum::HARD_RELOAD:
        os << "HARD_RELOAD";
        break;
    case iDomStateEnum::RASPBERRY_RELOAD:
        os << "RASPBERRY_RELOAD";
        break;
    default:
        os << "Unknown";
    }
    return os;
};

namespace iDomConst
{
constexpr int MAX_CONNECTION = 10;
constexpr int FREE = 1;
constexpr int RS232 = 11;
constexpr int CLOCK = 12;
constexpr int ok = 0;
constexpr int GPIO_SPIK = 21;
constexpr int GPIO_PRINTER = 22;
}

struct ALERT
{
    Clock time;
    STATE state = STATE::DEACTIVE;
    unsigned int fromVolume = 48;
    unsigned int toVolume = 58;
    unsigned int radioID = 8;
    std::vector<std::string> commands;
};

extern std::string _logfile;
extern Logger log_file_mutex;
extern std::string buffer;

enum class TEMPERATURE_STATE;

struct MPD_info
{
    std::string title = "NULL";
    std::string radio = "NULL";
    std::string artist = "NULL";
    int volume = 0;
    bool isPlay = false;
    int currentSongID = 0;
    std::vector<std::string> songList = {"NULL"};
};

struct Thread_array_struc
{
    std::thread thread;
    std::thread::id thread_ID = std::thread::id(0);
    std::string thread_name;
    unsigned int thread_socket = 0;
};

struct address_another_servers
{
    int id;
    std::string SERVER_IP;
};

struct FTP_SERVER
{
    std::string URL;
    std::string user;
    std::string pass;
};

struct iDOM_STATE
{
    STATE houseState = STATE::UNDEFINE;
    unsigned int counter = 0;
    STATE alarmSatelState = STATE::UNDEFINE;
};

struct GATEWAY
{
    std::string url = "NULL";
    unsigned int keySize = 0;
};

struct RunThread_CFG
{
    bool RFLink = false;
    bool MPD = false;
    bool CRON = false;
    bool RS232 = false;
    bool DUMMY = false;
    bool MQTT = false;
    bool SATEL = false;
    bool INFLUX = false;
};

struct RFLink_CFG
{
    std::string RFLinkPort;
    int RFLinkBaudRate;
};

struct RS232_CFG
{
    std::string portRS232;
    int BaudRate;
};

struct FB_AND_VIBER_CFG
{
    std::string viberToken = "NULL";
    std::string viberAvatar;
    std::vector<std::string> viberReceiver;
    std::string viberSender;
};

struct CONFIG_SERVER
{
    int PORT;
    std::string SERVER_IP;
    std::string MPD_IP;
    std::string TS_KEY = " gg ";
    std::string TS_KEY2 = " gg ";
    std::string radio433MHzConfigFile;
    int ID_server = 0;
    int v_delay;
    bool encrypted = true;
    bool viber_notification = true;
    FTP_SERVER ftpServer;
    std::string lightningApiURL = "NULL";
    std::string saveFilePath = "NULL";
    std::string keyDatabasePath = "NULL";
};

struct MQTT_BROKER_CFG
{
    std::string host = "localhost";
    std::string topicSubscribe = "iDom";
    std::string topicPublish = "iDom";
    int port = 1883;
    int qos = 2;
};

struct SATEL_INTEGRA_CFG
{
    std::string host = "null";
    int port = 7094;
    std::string pin = "null";
    unsigned int outdoor_siren_lights_id = 0;
    unsigned int partitionID = 1;
};

struct CONFIG_JSON
{
    RunThread_CFG _runThread;
    FB_AND_VIBER_CFG _fb_viber;
    CONFIG_SERVER _server;
    RS232_CFG _rs232;
    RFLink_CFG _rflink;
    MQTT_BROKER_CFG _mqtt_broker;
    GATEWAY _gateway;
    SATEL_INTEGRA_CFG _satel_integra;
    nlohmann::json _shedConf;
    nlohmann::json _command;
};

//class command; // for struc thread_data req
class iDomTOOLS_INTERFACE;
class RADIO_EQ_CONTAINER;
class RFLinkHandler;
class SATEL_INTEGRA_HANDLER_INTERFACE;
// TODO temporary
enum class PILOT_KEY
{
    KEY_POWER,
    KEY_AUDIO,
    KEY_EPG,
    KEY_OK,
    KEY_RADIO,
    KEY_TV,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_CHANNELDOWN,
    KEY_CHANNELUP,
    KEY_DOWN,
    KEY_EXIT,
    KEY_FAVORITES,
    KEY_INFO,
    KEY_LANGUAGE,
    KEY_MENU,
    KEY_MUTE,
    KEY_REFRESH,
    KEY_SAT,
    KEY_SUBTITLE,
    KEY_TEXT,
    KEY_UP,
    KEY_VOLUMEDOWN,
    KEY_VOLUMEUP,
    SLEEPER,
    DUMMY
};

enum class KEY_PAD : int
{
    POWER = 1,
    OK = 2,
    RES = 4,
    UP = 16,
    MENU = 32,
    RIGHT = 64,
    DOWN = 256,
    LEFT = 512,
    EPG = 1024,
    REBOOT = 5,
    OFF_LCD = 777,
    TIMEOUT = 778,
};

class KEY_HANDLER_BASE{
public:
    virtual ~KEY_HANDLER_BASE() = default;
    virtual void recKeyEvent(KEY_PAD eventId) = 0;
    virtual void recIrdaEvent(PILOT_KEY eventId) = 0;
    virtual void timeout() = 0;
    virtual void quickPrint(const std::string &row1, const std::string &row2) = 0;
    virtual void scrollText() = 0;
};

struct LUSINA
{
    nlohmann::json shedJson;
    nlohmann::json shedConfJson;
    STATISTIC<float> shedTemp;
    STATISTIC<float> shedHum;
    STATISTIC<float> shedFloor;
    STATISTIC<float> shedPres;
    STATISTIC<float> acdc;
    LUSINA() : shedTemp(18), shedHum(18), shedFloor(18), shedPres(18), acdc(18) {}
};

struct thread_data
{
    int s_client_sock;
    struct sockaddr_in from;
    struct CONFIG_JSON *server_settings = NULL;
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> *main_THREAD_arr = NULL;
    time_t start;
    time_t now_time;
    int sleeper;
    event_counters_handler myEventHandler;
    std::string encriptionKey = "40%";
    iDOM_STATE idom_all_state;
    ALERT alarmTime;
    iDomStateEnum iDomProgramState = iDomStateEnum::WORKING;
    alarm_C iDomAlarm;
    bool serverStarted = false;
    LUSINA lusina;
    BlockingQueue<std::any> dbDataQueue;
    SATEL_INTEGRA_HANDLER_INTERFACE *satelIntegraHandler = std::nullptr_t();
    std::unique_ptr<KEY_HANDLER_BASE> main_key_menu_handler = std::nullptr_t();
    std::shared_ptr<iDomTOOLS_INTERFACE> main_iDomTools = std::nullptr_t();
    std::unique_ptr<SerialPi> main_Rs232 = std::nullptr_t();
    std::shared_ptr<RFLinkHandler> main_RFLink = std::nullptr_t();
    std::shared_ptr<house_room_handler> main_house_room_handler = std::nullptr_t();
    std::unique_ptr<MPD_info> ptr_MPD_info = std::nullptr_t();
    std::shared_ptr<BUDERUS_IF> ptr_buderus = std::nullptr_t();
    std::unique_ptr<MQTT_mosquitto> mqttHandler = std::nullptr_t();
    std::shared_ptr<RADIO_EQ_CONTAINER> main_REC = std::nullptr_t();
    std::unique_ptr<iDomSTATUS> main_iDomStatus = std::nullptr_t();
    std::unique_ptr<iDomKEY_ACCESS> m_keyHandler = std::nullptr_t();
    ~thread_data() = default;
};

iDomStateEnum iDom_main();

