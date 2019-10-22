#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <string>
#include <mutex>

#include "../iDom_server_OOP.h"
#include "../c_connection/c_connection.h"

class useful_F {

public:
    volatile static unsigned int lastInterruptTime;
    static std::mutex mut;
    static std::string conv_dns(std::string temp);
    /////// not used now
    // void ChangeEndianness(int32_t * value, int MAX_MSG_LEN_INT);
    // void binary(int val);
    //std::string intToStr(int n);
    //////////////
    static CONFIG_JSON configJsonFileToStruct( nlohmann::json jj);
    static std::string send_to_arduino(thread_data *my_data_logic, const std::string &msg);
    static std::string send_to_arduino_clock(thread_data *my_data_logic, std::string msg);
    static std::string sek_to_uptime(long long secy );
    static void sleeper_mpd(thread_data* my_data, const std::string &threadName);
    static void kodi(thread_data* my_data, const std::string &threadName);
    static void tokenizer(std::vector <std::string> &command, std::string separator, const std::string &text);
    static std::string RSHash(const std::string& data ="", unsigned int b = 378551, unsigned int a = 63689);
    static std::string l_send_file(std::string path, std::string find, bool reverse =false );
    static std::vector<std::string> split(const std::string& s, char separator );
    static bool go_while;
    static bool workServer;
    static thread_data* myStaticData;
    static void setStaticData(thread_data* my_dataPtr);
   //S static void clearThreadArray(thread_data *my_data);

    static int runLinuxCommand(const std::string& cmd);

    static std::mutex mutex_buf;
    static std::mutex mutex_who;
    static void sleep(int sec);
};
#endif // FUNCTIONS_H
