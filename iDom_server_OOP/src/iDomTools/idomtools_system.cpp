#include "idomtools.h"
#include <sys/sysinfo.h>

#include "../433MHz/RFLink/rflinkhandler.h"
#include "../functions/functions.h"

std::string iDomTOOLS::getSystemInfo()
{
    double load[3];
    if (getloadavg(load, 3) not_eq -1)
    {
#ifdef BT_TEST
        printf("load average : %f , %f , %f\n", load[0], load[1], load[2]);
#endif
    }
    const double megabyte = 1024 * 1024;
    struct sysinfo info;
    sysinfo(&info);

    long input_seconds = info.uptime;
    auto days = input_seconds / 60 / 60 / 24;
    auto hours = (input_seconds / 60 / 60) % 24;
    auto minutes = (input_seconds / 60) % 60;
    auto seconds = input_seconds % 60;

    std::stringstream ret;
    ret << "System uptime: " << days << " day " << hours
        << " hours " << minutes << " minutes "
        << seconds << " seconds " << std::endl
        << "Load: "
        << (info.loads[0] / 65536) << "% - 1 min, " << (info.loads[1] / 65536)
        << "% - 5 min, " << (info.loads[2] / 65536) << "% - 15 min." << std::endl
        << "process count : " << info.procs << std::endl
        << "total RAM   : " << (info.totalram / megabyte) << "MB" << std::endl
        << "free RAM   : " << (info.freeram / megabyte) << "MB" << std::endl;

    return ret.str();
}

/////////////////////// health check //////////////////
void iDomTOOLS::healthCheck()
{
    int alarm_433MHz = 2131;

    if (context->mqttHandler->_connected == false)
    {
        puts("brak polaczenia z serverm MQTT");
    }
    if (context->mqttHandler->_subscribed == false)
    {
        puts("brak subskrybcji do servera mqtt");
    }

    ////////////// RFLink ///////////
    auto t = Clock::getUnixTime() - context->main_RFLink->m_pingTime;

    if (t > 310 and context->mqttHandler->_subscribed == true and context->server_settings->_runThread.RFLink == true)
    {
        m_restartAlarmRFLink++;
        if (m_restartAlarmRFLink == 4)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << WARNING << "restart servera z powodu braku polaczenia z RFLinkiem" << std::endl;
            log_file_mutex.mutex_unlock();
            context->main_iDomTools->reloadHard_iDomServer();
        }
        else if (m_restartAlarmRFLink == 3)
        {
            context->main_RFLink = std::make_shared<RFLinkHandler>(context);
            useful_F::sleep(1s);
            context->main_RFLink->init();

            // context->main_RFLink->sendCommand("10;REBOOT;");
        }
        context->main_RFLink->sendCommand("10;PING;");
        context->main_RFLink->sendCommand("10;PING;");
        context->main_RFLink->sendCommand("10;PING;");
        std::string m("brak połaczenia RS232 z RFLink'iem");
        std::cout << "brak pingu RFLinka 433MHz t: " << Clock::unixTimeToString(t) << std::endl;
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "braku polaczenia z RFLinkiem" << std::endl;
        log_file_mutex.mutex_unlock();
        context->iDomAlarm.raiseAlarm(alarm_433MHz, m);
        context->main_iDomTools->sendViberMsg(m, context->server_settings->_fb_viber.viberReceiver.at(0),
                                              context->server_settings->_fb_viber.viberSender + "health");
    }
    else
    {
        context->iDomAlarm.clearAlarm(alarm_433MHz);
        m_restartAlarmRFLink = 0;
    }
}

std::string iDomTOOLS::dump() const
{
    std::stringstream ret;
    ret << std::boolalpha;
    ret << "m_viber_notif: " << m_viber_notif << std::endl;
    ret << " m_restartAlarmRFLink: " << this->m_restartAlarmRFLink << std::endl;
    ret << " key: " << this->m_key << std::endl;
    ret << " lastButton433MHzLockUnlockTime: " << this->m_lastButton433MHzLockUnlockTime << std::endl;
    return ret.str();
}

std::string iDomTOOLS::getLink(std::vector<std::string> v)
{
    generator gg;
    std::string tempName = gg.random_string(20);
    auto command = std::accumulate(std::begin(v), std::end(v), std::string(), [](const std::string &lhs, const std::string &rhs)
                                   { return lhs.empty() ? rhs : lhs + ' ' + rhs; });
    context->m_keyHandler->addTempKEY(tempName, command, context->server_settings->_gateway.keySize);
    std::string key = context->m_keyHandler->getKEY(tempName);
    std::stringstream ret;

    ret << context->server_settings->_gateway.url << "?name=" << tempName << "&key=" << key;

    for (const auto &s : v)
    {
        ret << "&" << s;
    }

    return ret.str();
}

void iDomTOOLS::reloadSoft_iDomServer()
{
    context->iDomProgramState = iDomStateEnum::RELOAD;
    context->main_iDomTools->saveState_iDom(context->serverStarted);
    useful_F::workServer = false;
}

void iDomTOOLS::reloadHard_iDomServer()
{
    context->iDomProgramState = iDomStateEnum::HARD_RELOAD;
    context->main_iDomTools->saveState_iDom(context->serverStarted);
    useful_F::workServer = false;
}

void iDomTOOLS::raspberryReboot()
{
    iDomTOOLS::MPD_stop();
    context->iDomProgramState = iDomStateEnum::RASPBERRY_RELOAD;
    context->main_iDomTools->saveState_iDom(context->serverStarted);
    useful_F::workServer = false;
}

void iDomTOOLS::uploadRamCpuUsage()
{
    auto cpu = useful_F_libs::getCpuUsage();
    auto memory = useful_F_libs::getMemoryUsageInMB();
    uint64_t timestamp = Clock::getTimestamp();

    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> iDomData;

    try
    {
        iDomData["system"]["RAM"] = memory;
        iDomData["system"]["CPU"] = cpu;

        dbClientFactory dbFactory;
        auto db = dbFactory.createDbClient();
        auto returnCode = db->upload_iDomData(iDomData, timestamp, &context->server_settings->_database2);
        if (returnCode != 204)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << CRITICAL << " błąd wysyłania danych iDom do influxdb " << returnCode << " " << reasonPhrase(returnCode) << std::endl;
            log_file_mutex.mutex_unlock();

            std::exception kk;
            throw kk;
        }
    }
    catch (std::exception &e)
    {
        // context->dbDataQueue.Put(DB_DATA(timestamp, iDomData)); // put data to queue, send later
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << " błąd (wyjatek) wysyłania memory do influxdb " << e.what() << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void iDomTOOLS::close_iDomServer()
{
    iDomTOOLS::MPD_stop();
    context->iDomProgramState = iDomStateEnum::CLOSE;
    context->main_iDomTools->saveState_iDom(context->serverStarted);
    useful_F::workServer = false;
}
