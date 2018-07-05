#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <vector>

#include "functions.h"

std::vector<std::string> useful_F::split(const std::string& s, char separator ){
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    try
    {
        output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    }
    catch (...)
    {
#ifndef BT_TEST
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek substr() w useful_F::split() !!!!!!"<< std::endl;
        log_file_mutex.mutex_unlock();
#endif
    }
    return output;
}
thread_data* useful_F::myStaticData = NULL;
void useful_F::setStaticData(thread_data *my_dataPtr)
{
    myStaticData = my_dataPtr;
}

void useful_F::tokenizer ( std::vector <std::string> &command, std::string separator, std::string &text){
    std::string temp;

    for(char n: text)
    { // the initializer may be an array
        bool is_sep = false;
        for(char m: separator)
        {
            if (n == m)
            {
                is_sep = true;
            }
        }
        if (is_sep == false)
        {
            temp += n;
        }
        else
        {
            if (!temp.empty())
            {
                command.push_back( temp);
                temp = "";
            }
        }
    }
    if (!temp.empty())
    {
        command.push_back(temp);
    }
}

////// watek sleeper
void useful_F::sleeper_mpd (thread_data  *my_data)
{
    unsigned int t = 60/my_data->sleeper;
    unsigned int k = 0;

    for (; my_data->sleeper >0 ; my_data->sleeper-- )
    {
        useful_F::sleep_1min();
        k += t;
        my_data->main_iDomTools->ledClear(0,k);
    }
    my_data->main_iDomTools->ledOFF();
    my_data->main_iDomTools->MPD_stop();
    my_data->main_iDomTools->turnOff433MHzSwitch("listwa");
#ifndef BT_TEST
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< "zaczynam procedure konca watku SLEEP_MPD" <<  std::endl;
    log_file_mutex.mutex_unlock();
#endif
    try
    {
        for (int i =0 ; i< iDomConst::MAX_CONNECTION;++i)
        {
            if (my_data->main_THREAD_arr[i].thread_ID == std::this_thread::get_id())
            {
                //my_data->main_THREAD_arr[i].thread.detach();
                my_data->main_THREAD_arr[i].thread_name ="  -empty-  ";
                my_data->main_THREAD_arr[i].thread_ID =  std::thread::id();
                my_data->main_THREAD_arr[i].thread_socket = 0;
                break;
            }
        }
    }
    catch (std::system_error &e)
    {
#ifndef BT_TEST
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR<< "zlapano wyjatek w  watku SLEEP_MPD: " << e.what()<< std::endl;
        log_file_mutex.mutex_unlock();
#endif
    }
#ifndef BT_TEST
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< "koniec  watku SLEEP_MPD" <<  std::endl;
    log_file_mutex.mutex_unlock();
#endif
}

std::string useful_F::RSHash(std::string data, unsigned int b, unsigned int a)
{
    time_t act_time;
    struct tm * act_date;
    time(&act_time);
    act_date = localtime(&act_time);
    char buffer[10];
    strftime(buffer,10,"%M%H%w",act_date);
    std::string str(buffer);
    str+=data;
    unsigned int hash = 0;

    for(std::size_t i = 0; i < str.length(); i++)
    {
        hash = hash * a + str[i];
        a    = a * b;
    }
    return std::to_string((hash & 0x7FFFFFFF));
}

