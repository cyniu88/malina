#ifndef CRON_HPP
#define CRON_HPP
#include <iostream>
#include <time.h>
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
    void runEveryone_1min(struct tm *act_date);
    void runEveryone_5min();
    void runEveryone_15min();
    void runEveryone_30min();
    void runEveryone_1h();
};

#endif // CRON_HPP
