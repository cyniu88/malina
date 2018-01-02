#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <string>
#include "../iDom_server_OOP.h"
#include "../c_connection/c_connection.h"

class useful_F {

public:
    volatile static  unsigned int lastInterruptTime;
    static std::mutex mut;
    static  std::string conv_dns (std::string temp);
    /////// not used now
    // void ChangeEndianness(int32_t * value, int MAX_MSG_LEN_INT);
    // void binary(int val);
    //std::string intToStr(int n);
    //////////////
    static  config configFileToStruct ();
    static  std::string send_to_arduino (thread_data *my_data_logic, std::string msg);
    static  std::string send_to_arduino_clock (thread_data *my_data_logic, std::string msg);
    static  std::string  sek_to_uptime(long long secy );
    static  void sleeper_mpd(thread_data  *my_data);
    static  void tokenizer ( std::vector <std::string> &command, std::string separator, std::string &text);
    static  std::string  RSHash(std::string data ="", unsigned int b = 378551,  unsigned int a = 63689);
    static  void write_to_mkfifo(std::string msg);
    static  std::string read_from_mkfifo();
    static  std::string l_send_file(std::string path, std::string find  , bool reverse =false );
    static  std::vector<std::string> split(const std::string& s, char separator );
    static  void button_interrupt();
    static  bool go_while ;
    static  thread_data* myStaticData;
    static  void setStaticData(thread_data* my_dataPtr);
    static void clearThreadArray(thread_data *my_data);
    static int findFreeThreadSlot(Thread_array_struc *array);
    //////////////////// HTTP req //////////////////////////
    static std::string httpPost(std::string url, int timeoutSeconds );
    static void downloadFile(std::string url, std::string path, int timeoutSeconds);

};
#endif // FUNCTIONS_H
