#ifndef CRON_HPP
#define CRON_HPP
#include <iostream>
#include <time.h>
#include <string>
#include"../functions/functions.h"

class CRON
{
public:
    explicit CRON( thread_data* my_data);
    void run();
private:
    CRON();
    bool check_temperature;
    thread_data * my_data;
    void send_time();
    void runEveryone_1min();
    void runEveryone_5min();
    void runEveryone_15min();
    void runEveryone_30min();
    void runEveryone_1h();
    void runOnSunset();
    void runOnSunrise();
};

#endif // CRON_HPP
