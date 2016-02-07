#ifndef CRON_HPP
#define CRON_HPP
#include <iostream>
#include <time.h>
#include <curl/curl.h>
#include"../functions/functions.h"

class CRON
{
public:
    CRON( thread_data * my_data);
    void run();
    void  send_temperature_thingSpeak(std::string key);
private:

    bool check_temperature;
    thread_data * my_data;
};

#endif // CRON_HPP
