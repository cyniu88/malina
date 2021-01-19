#ifndef GLOBAL_H
#define GLOBAL_H

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
#include <wiringPi.h>
#include <chrono>
#include <array>

// MOJE BIBLIOTEKI
#include "logger/logger.hpp"
#include "command/command.h"
#include "../libs/event_counters/event_counters_handler.h"
#include "iDomTools/idomtools.h"
#include "iDomStatus/idomstatus.h"
#include "iDomSaveState/idom_save_state.h"
#include "../mqtt_mosquitto/MQTT_mosquitto/mqtt.h"
#include "../libs/alarm/alarm.h"
#include "buderus/buderus.h"
#include "home_lighting/house_lighting_handler.h"
#include "SerialPi/serialpi.h"
//#include "MENU/menu_base.h"

#define log_file_cout  f_log //std::cout zmien f_log na std::cout i bedzie wypisywac na ekran
#define log_file_mutex f_log

enum class color : int{
    orange = 0,
    red = 1,
    green = 2,
    blue = 3,
    white = 4,
    yellow = 5,
    cyan = 6,
    magenta = 7,
    dark_orange = 8,
    //NOTE update std::unordered_map<std::string, color> stringToColor below
};


const std::unordered_map<std::string, color> stringToColor = {
    {"orange", color::orange},
    {"red", color::red},
    {"green", color::green},
    {"blue", color::blue},
    {"white", color::white},
    {"yellow", color::yellow},
    {"cyan", color::cyan},
    {"magenta", color::magenta},
    {"dark_orange", color::dark_orange}
};

enum class iDomStateEnum{
    CLOSE = 0,
    RELOAD,
    ERROR,
    WORKING,
    HARD_RELOAD
};

namespace iDomConst
{
constexpr int MAX_CONNECTION = 10;
constexpr int FREE  = 1;
constexpr int RS232 = 11;
constexpr int CLOCK = 12;
constexpr int ok    = 0;
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
};

extern std::string _logfile;
extern Logger log_file_mutex;
extern std::string buffer;

enum class TEMPERATURE_STATE;

struct MPD_info{
    std::string title   = "NULL";
    std::string radio   = "NULL";
    std::string artist  = "NULL";
    int volume = 0;
    bool isPlay = false;
    int currentSongID = 0;
    std::vector <std::string> songList = {"NULL"};
};

struct Thread_array_struc {
    std::thread thread;
    std::thread::id thread_ID = std::thread::id(0);
    std::string thread_name;
    unsigned int thread_socket = 0;
};

struct address_another_servers {
    int id;
    std::string SERVER_IP;
};

struct FTP_SERVER{
    std::string URL;
    std::string user;
    std::string pass;
};
struct iDOM_STATE{
    STATE houseState = STATE::UNDEFINE;
};

struct GATEWAY{
    std::string url = "NULL";
    unsigned int keySize = 0;
};

struct RunThread_CFG{
    bool RFLink = false;
    bool MPD   = false;
    bool CRON  = false;
    bool RS232 = false;
    bool DUMMY = false;
    bool MQTT  = false;
};

struct CAMERA_CFG{
    std::string cameraLedON = "";
    std::string cameraLedOFF = "";
    std::string cameraURL = "";
};

struct RFLink_CFG{
    std::string RFLinkPort;
    int RFLinkBaudRate;
};

struct RS232_CFG{
    std::string portRS232;
    int BaudRate;
};

struct FB_AND_VIBER_CFG{
    std::string facebookAccessToken = "";
    std::string viberToken = "NULL";
    std::string viberAvatar;
    std::vector <std::string> viberReceiver;
    std::string viberSender;
};

struct CONFIG_SERVER{
    int PORT;
    std::string SERVER_IP;
    std::string MPD_IP;
    std::string TS_KEY= " gg ";
    std::string radio433MHzConfigFile;
    int ID_server = 0;
    int v_delay;
    bool encrypted = true;
    FTP_SERVER ftpServer;
    std::string lightningApiURL = "NULL";
    std::string saveFilePath = "NULL";
    std::string keyDatabasePath = "NULL";
};

struct MQTT_BROKER_CFG{
    std::string host = "localhost";
    std::string topicSubscribe = "iDom";
    std::string topicPublish = "iDom";
    int port = 1883;
    int qos  = 2;
};

struct CONFIG_JSON{
    RunThread_CFG _runThread;
    CAMERA_CFG _camera;
    FB_AND_VIBER_CFG _fb_viber;
    CONFIG_SERVER _server;
    RS232_CFG _rs232;
    RFLink_CFG _rflink;
    MQTT_BROKER_CFG _mqtt_broker;
    GATEWAY _gateway;
};



class command; // for struc thread_data req
class iDomTOOLS;
class RADIO_EQ_CONTAINER;
class RFLinkHandler;
class BUDERUS;
class KEY_HANDLER;
//TODO temporary

struct LUSINA{
    std::string temperatureDS20;
    std::string temperatureDTH;
    std::string humidityDTH;
    STATISTIC <int>statHumi;
    STATISTIC <float>statTemp;
    LUSINA(): statHumi(270),statTemp(270){}
};

struct thread_data{
    int s_client_sock;
    struct sockaddr_in from;
    struct CONFIG_JSON *server_settings = NULL;
    //std::unique_ptr<KEY_HANDLER> main_key_menu_handler = std::nullptr_t();
    std::unique_ptr<iDomTOOLS> main_iDomTools = std::nullptr_t();
    std::unique_ptr<SerialPi> main_Rs232 = std::nullptr_t();
    std::shared_ptr<RFLinkHandler> main_RFLink = std::nullptr_t();
    std::shared_ptr<house_lighting_handler> main_house_lighting_handler = std::nullptr_t();
    std::array<Thread_array_struc, iDomConst::MAX_CONNECTION> *main_THREAD_arr = NULL;
    time_t start;
    time_t now_time;
    int sleeper;
    std::unique_ptr<MPD_info> ptr_MPD_info = std::nullptr_t();
    std::unique_ptr<BUDERUS> ptr_buderus = std::nullptr_t();
    event_counters_handler myEventHandler;
    std::string encriptionKey = "40%";
    std::unique_ptr<iDomSTATUS> main_iDomStatus;
    iDOM_STATE idom_all_state;
    ALERT alarmTime;
    std::shared_ptr<RADIO_EQ_CONTAINER> main_REC;
    iDomStateEnum iDomProgramState = iDomStateEnum::WORKING;
    std::unique_ptr<MQTT_mosquitto> mqttHandler = std::nullptr_t();
    alarm_C iDomAlarm;
    bool serverStarted = false;
    LUSINA lusina;
   // std::unique_ptr<MENU_STATE_BASE> main_menu = std::make_unique<MENU_MAIN>();
};

iDomStateEnum iDom_main();

#endif // GLOBAL_H
