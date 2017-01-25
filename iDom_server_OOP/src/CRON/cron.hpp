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
private:
    bool check_temperature;
    thread_data * my_data;
    void send_time();
};

#endif // CRON_HPP
