#include "cron.hpp"

CRON::CRON(thread_data *my_data)
{
    this->my_data = my_data;
    this->check_temperature = true;
}

void CRON::run()
{
    time_t act_time;
    struct tm *act_date;
    int min = 0;

    while (useful_F::go_while)
    {
        sleep(10);
        time(&act_time);
        act_date = localtime(&act_time);

        if (min != act_date->tm_min && useful_F::go_while)
        {
            try
            {

                runEveryone_1min(act_date);
                if (act_date->tm_min % 5 == 0)
                {
                    runEveryone_5min();
                }
                if (act_date->tm_min % 15 == 0)
                {
                    runEveryone_15min();
                }
                if (act_date->tm_min % 30 == 0)
                {
                    runEveryone_30min();
                }
                if (act_date->tm_min == 0)
                {
                    runEveryone_1h();
                }
            }
            catch (...)
            {
                log_file_mutex.mutex_lock();
                log_file_cout << ERROR << "wyjatek w cron run() " << std::endl;
                log_file_mutex.mutex_unlock();
            }
            min = act_date->tm_min;
        }
    }
}

void CRON::runEveryone_1min(struct tm *act_date)
{
    char buffer2[5];
    strftime(buffer2, 5, "%H%M", act_date);
    std::string msg = "CLOCK:";
    msg.append(buffer2);
    msg.append(";");
    my_data->main_Rs232->print(msg);
    my_data->main_iDomTools->checkAlarm();
    my_data->main_iDomTools->updateTemperatureStats();
    my_data->ptr_buderus->circlePompToRun();

    auto now = Clock::getTime();
    if (now == my_data->main_iDomTools->getSunsetClock())
    {
        runOnSunset();
    }
    if (now == my_data->main_iDomTools->getSunriseClock())
    {
        runOnSunrise();
    }
}

void CRON::runEveryone_5min()
{
    auto topic = my_data->server_settings->_mqtt_broker.topicSubscribe;
    topic.pop_back();
    my_data->mqttHandler->publish(topic + "command", "433MHz send 10;PING;");

    try
    {
        my_data->main_iDomTools->checkLightning();
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "wyjatek w pobieraniu jsona z piorunami " << std::endl;
        log_file_mutex.mutex_unlock();
    }

    my_data->main_iDomTools->healthCheck();
    my_data->main_house_room_handler->turnOffUnexpectedBulb();
}

void CRON::runEveryone_15min()
{
    my_data->main_iDomTools->send_data_to_thingSpeak();
}

void CRON::runEveryone_30min()
{
    // printf("co 30 minut! \n");
}

void CRON::runEveryone_1h()
{
    my_data->myEventHandler.clearOld(8000, 1000, [](const std::string &name)
                                     {
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "skasowano nadmiarowe eventy w: "<< name << std::endl;
        log_file_mutex.mutex_unlock(); });
}

void CRON::runOnSunset()
{
    log_file_mutex.mutex_lock();
    log_file_cout << DEBUG << "zachod slonca " << std::endl;
    log_file_mutex.mutex_unlock();

    my_data->main_iDomTools->runOnSunset();
}

void CRON::runOnSunrise()
{
    log_file_mutex.mutex_lock();
    log_file_cout << DEBUG << "wschod slonca" << std::endl;
    log_file_mutex.mutex_unlock();
    my_data->main_iDomTools->runOnSunrise();
}
