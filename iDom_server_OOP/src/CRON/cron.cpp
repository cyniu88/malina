#include "cron.hpp"
#include "../functions/functions.h"
#include "../iDomTools/idomtools_interface.h"
#include "../dbClient/db-client-factory.hpp"

CRON::CRON(thread_context *context) : context(context)
{

    // try
    // {
    //     rootConfig.loadFromFile(context->server_settings->_cron);

    //     for (const auto &[time, commands] : rootConfig.getCommands())
    //     {
    //         std::cout << "[" << time << "]\n";
    //         std::cout << "  Lock:\n";
    //         for (const auto &cmd : commands.lock)
    //             std::cout << cmd << "\n";
    //         std::cout << "  Unlock:\n";
    //         for (const auto &cmd : commands.unlock)
    //             std::cout << cmd << "\n";
    //     }

    //     commandData = rootConfig.getCommands();
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << "Błąd: " << e.what() << std::endl;
    // }
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
    context->main_Rs232->print(msg);
    context->main_iDomTools->checkAlarm();
    context->main_iDomTools->updateTemperatureStats();
    context->ptr_buderus->circlePompToRun();

    auto now = Clock::getTime();
    if (now == context->main_iDomTools->getSunsetClock())
    {
        runOnSunset();
    }
    if (now == context->main_iDomTools->getSunriseClock())
    {
        runOnSunrise();
    }
    runCommandCron("1h");
}

void CRON::runEveryone_5min()
{
    auto topic = context->server_settings->_mqtt_broker.topicSubscribe;
    topic.pop_back();
    context->mqttHandler->publish(topic + "command", "433MHz send 10;PING;");
    try
    {
        context->main_iDomTools->checkLightning();
    }
    catch (...)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "wyjatek w pobieraniu jsona z piorunami " << std::endl;
        log_file_mutex.mutex_unlock();
    }

    context->main_iDomTools->healthCheck();
    context->main_house_room_handler->turnOffUnexpectedBulb();

    context->main_iDomTools->send_data_to_influxdb();
    context->main_iDomTools->uploadRamCpuUsage();
    context->main_iDomTools->uploadRecuperatorData();

    if (context->server_settings->_command.contains("5min"))
    {
        context->main_iDomTools->runCommandFromJson(context->server_settings->_command["sunset"]["lock"].get<std::vector<std::string>>());
    }
    
    runCommandCron("5min");
}

void CRON::runEveryone_15min()
{
    context->main_iDomTools->send_data_to_thingSpeak();
    runCommandCron("15min");
}

void CRON::runEveryone_30min()
{
    runCommandCron("30min");
}

void CRON::runEveryone_1h()
{
    context->myEventHandler.clearOld(8000, 1000, [](const std::string &name)
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

    context->main_iDomTools->runOnSunset();
}

void CRON::runOnSunrise()
{
    log_file_mutex.mutex_lock();
    log_file_cout << DEBUG << "wschod slonca" << std::endl;
    log_file_mutex.mutex_unlock();
    context->main_iDomTools->runOnSunrise();
}

void CRON::runCommandCron(const std::string &time)
{   
    std::cout << "cyniu runCommandCron start  " << time << std::endl;
    if (context->idom_all_state.houseState == STATE::LOCK)
    {
        auto v = commandData.at(time).lock;
        for(const auto &cmd : v)
        {
            std::cout << cmd << std::endl;
        }
            context->main_iDomTools->runCommandFromJson(v);
    }
    else if (context->idom_all_state.houseState == STATE::UNLOCK)
    {
        auto v = commandData.at(time).unlock;
    }
    
    std::cout << "cyniu runCommandCron stop " << time << std::endl;
}
