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

// MOJE BIBLIOTEKI
#include "KEY/key.h"
#include "logger/logger.hpp"
#include "files_tree/files_tree.h"
#include "menu_tree/menu_tree.h"
#include "LCD_c/lcd_c.h"
#include "command/command.h"
#include "../libs/event_counter_test/event_counters/event_counters_handler.h"
#include "iDomTools/idomtools.h"

#define log_file_cout  f_log //std::cout   zmien f_log na std::cout  i bedzie wypisywac na ekran
#define log_file_mutex f_log

namespace iDomConst
{
constexpr int MAX_CONNECTION = 10;
constexpr int FREE  = 1;
constexpr int RS232 = 11;
constexpr int CLOCK = 12;
constexpr int ok    =  0;
constexpr int GPIO_SPIK  = 21;
constexpr int BUTTON_PIN = 25;
}

extern std::string  _logfile  ;
extern Logger log_file_mutex;
extern std::string buffer;

enum class TEMPERATURE_STATE;

struct MPD_info{
    std::string title   = "NULL";
    std::string radio   = "NULL";
    std::string artist  = "NULL";
    int volume  = 0;
    bool isPlay = false;
    int currentSongID = 0;
    std::vector <std::string> songList = {"NULL"};
};
struct s_pointer{
    unsigned int *ptr_who;
    int32_t *ptr_buf;
};

struct Thread_array_struc {
    std::thread thread;
    std::thread::id thread_ID;
    std::string thread_name;
    int thread_socket;
};

struct address_another_servers {
    int id;
    std::string SERVER_IP;
};

struct config{
    std::string portRS232;
    std::string portRS232_clock;
    std::string BaudRate;
    std::string PORT;
    std::string SERVER_IP;
    std::string MPD_IP;
    std::string MOVIES_DB_PATH;
    std::string MENU_PATH;
    unsigned int ID_server;
    std::string TS_KEY= " gg ";
    int v_delay  ;
};

struct LED_Strip{
    std::string from ;
    std::string to ;
    std::string R ;
    std::string G ;
    std::string B ;
    std::string colorName;

    LED_Strip (int from, int to, int r, int g, int b, std::string colorName = "NULL"):from(std::to_string(from)),
        to(std::to_string(to)),
        R(std::to_string(r)),
        G(std::to_string(g)),
        B(std::to_string(b)),
        colorName(colorName)
    {

    }
    LED_Strip (std::string from, std::string to, std::string r, std::string g, std::string b, std::string colorName = "NULL"):
        from(from),
        to(to),
        R(r),
        G(g),
        B(b),
        colorName(colorName)
    {

    }

    void set (std::string from, std::string to, std::string r, std::string g, std::string b, std::string colorName = "NULL"){
        this->from =from;
        this->to = to;
        R = r;
        G = g;
        B = b;
        this->colorName =colorName;
    }

    void set (int from, int to, int r, int g, int b, std::string colorName = "NULL"){
        this->from =std::to_string(from);
        this->to = std::to_string(to);
        R = std::to_string(r);
        G = std::to_string(g);
        B = std::to_string(b);
        this->colorName =colorName;
    }

    std::string getColorName() const{
        return colorName;
    }

    std::string get() const{
        return "LED:["+from+"-"+to+"-"+R+"-"+G+"-"+B+"];";
    }

    static std::string makeCommand(std::string from, std::string to, std::string R, std::string G, std::string B){
        return "LED:["+from+"-"+to+"-"+R+"-"+G+"-"+B+"];";
    }
};

struct pilot_led{
    unsigned int counter=0;
    std::vector<LED_Strip> colorLED   = { LED_Strip(1,60,237,145,33 ,"carrot orange"),
                                          LED_Strip(1,60,255,0,0    ,"red"),
                                          LED_Strip(1,60,0,255,0    ,"green"),
                                          LED_Strip(1,60,0,0,255    ,"blue"),
                                          LED_Strip(1,60,255,255,255,"white"),
                                          LED_Strip(1,60,255,255,0  ,"yellow"),
                                          LED_Strip(1,60,0,255,255  ,"cyan"),
                                          LED_Strip(1,60,255,0,255  ,"magenta")
                                        };
};

class command ;  // for struc thread_data req
class iDomTOOLS;

struct thread_data{
    int s_client_sock;
    struct sockaddr_in from;
    struct config *server_settings = NULL;
    struct s_pointer pointer;
    LCD_c *mainLCD = NULL;
    files_tree *main_tree = NULL;
    menu_tree *main_MENU = NULL;
    iDomTOOLS *main_iDomTools = NULL;
    Thread_array_struc *main_THREAD_arr = NULL;
    time_t start;
    time_t now_time;
    int sleeper;
    std::map <std::string, std::unique_ptr <KEY>  > key_map;
    MPD_info *ptr_MPD_info = NULL;
    pilot_led * ptr_pilot_led = NULL;
    std::map <std::string, std::unique_ptr<command> >* commandMapPtr = NULL;
    event_counters_handler myEventHandler;
    std::string encriptionKey = "40%";
};

struct thread_data_rs232{
    std::string portRS232;
    std::string portRS232_clock;
    std::string BaudRate;
    struct s_pointer pointer;
};

#endif // GLOBAL_H
