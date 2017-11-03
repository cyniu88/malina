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
    void runEveryOne1min(struct tm *act_date);
    void runEveryOne5min();
    void runEveryOne15min();
    void runEveryOne30min();
    void runEveryOne1h();
};

#endif // CRON_HPP
