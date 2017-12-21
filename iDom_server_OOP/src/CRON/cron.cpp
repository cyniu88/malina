#include "cron.hpp"

CRON::CRON(thread_data *my_data)
{
    this->my_data = my_data;
    this->check_temperature=TRUE;
}


void CRON::run()
{
    time_t act_time;
    struct tm * act_date;
    int min = 0 ;

    while (useful_F::go_while) {
        sleep(10);
        time(&act_time);
        act_date = localtime(&act_time);

        if (min != act_date->tm_min  )
        {
            runEveryone_1min(act_date);
            if (act_date->tm_min % 5 == 0 )
            {
                runEveryone_5min();
            }
            if (act_date->tm_min % 15 == 0 )
            {
                runEveryone_15min();
            }
            if (act_date->tm_min % 30 == 0 )
            {
                runEveryone_30min();
            }
            if (act_date->tm_min  == 0 )
            {
                runEveryone_1h();
            }
            min = act_date->tm_min;
        }
    }
}

void CRON::runEveryone_1min(struct tm *act_date)
{
    char buffer[5];
    strftime(buffer,5,"%H%M",act_date);
    useful_F::send_to_arduino_clock(my_data,buffer);
    my_data->main_iDomTools->checkAlarm();
    my_data->main_iDomTools->updateTemperatureStats();
}

void CRON::runEveryone_5min()
{
   // printf("co 5 minut! \n");
}

void CRON::runEveryone_15min()
{
  //  printf("co 15 minut! \n");
    my_data->main_iDomTools->send_temperature_thingSpeak();
}

void CRON::runEveryone_30min()
{
   // printf("co 30 minut! \n");
}

void CRON::runEveryone_1h()
{
  //  printf("co godzine! \n");
}
