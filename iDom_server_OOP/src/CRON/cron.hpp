#pragma once

#include <iostream>
#include <string>
#include <time.h>
struct thread_context;

class CRON
{
public:
    explicit CRON(thread_context *context);
    void run();

private:
    CRON();
    thread_context *context;
    void runEveryone_1min(tm *act_date);
    void runEveryone_5min();
    void runEveryone_15min();
    void runEveryone_30min();
    void runEveryone_1h();
    void runOnSunset();
    void runOnSunrise();
};

