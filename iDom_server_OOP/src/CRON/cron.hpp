#ifndef CRON_HPP
#define CRON_HPP
#include <iostream>
#include <time.h>
#include <curl/curl.h>
#include <string>
#include"../functions/functions.h"

class CRON
{
public:
    CRON( thread_data * my_data);
    void run();

    void  send_temperature_thingSpeak();
    static std::string smog( );
    static std::string find_tag (std::string temp);
private:

    bool check_temperature;
    thread_data * my_data;
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    void send_time();
    std::string key = "47XSQ0J9CPJ4BO2O";
};

#endif // CRON_HPP
